#version 450

layout(location = 0) in vec2 texCoords;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outHistory;
layout(location = 2) out vec2 outMoments;
layout(location = 3) out vec4 outDirectLight;

layout(binding = 0, std140) uniform UniformBufferObject2{
    vec3 cameraPos;
    vec3 lightDir;
    vec3 lightRadiance;
    mat4 world2clip;
    mat4 lightVP;
    mat4 prevWorld2Clip;
    uint temporalFrameIndex;
    float historyValid;
    float maxAccumFrames;
} ubo2;

#define M_PI 3.1415926535897932384626433832795
#define TWO_PI 6.283185307
#define INV_PI 0.31830988618
#define INV_TWO_PI 0.15915494309
#define SAMPLE_NUM 10
#define STEP_SIZE 0.2
#define ACC_STEP_SIZE 0.2

layout(binding = 1) uniform sampler2D gAlbedoSampler;
layout(binding = 2) uniform sampler2D gWorldPositionSampler;
layout(binding = 3) uniform sampler2D gWorldNormalSampler;
layout(binding = 4) uniform sampler2D gDepthSampler;
layout(binding = 5) uniform sampler2D smSampler;
layout(binding = 6) uniform sampler2D historySampler;
layout(binding = 7) uniform sampler2D momentsSampler;

// A reprojected sample is accepted while the depth it was written with still matches the
// surface we are shading, expressed as a fraction of that depth.
#define HISTORY_DEPTH_TOLERANCE 0.05

float Rand1(inout float p) {
    p = fract(p * .1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

vec2 Rand2(inout float p) {
    float x = Rand1(p);
    float y = Rand1(p);
    return vec2(x, y);
}

float InitRand(vec2 uv) {
    vec3 p3  = fract(vec3(uv.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

vec3 SampleHemisphereUniform(inout float s, out float pdf) {
    vec2 uv = Rand2(s);
    float z = uv.x;
    float phi = uv.y * TWO_PI;
    float sinTheta = float(sqrt(1.0 - z*z));
    vec3 dir = vec3(sinTheta * cos(phi), sinTheta * sin(phi), z);
    pdf = INV_TWO_PI;
    return dir;
}

vec3 SampleHemisphereCos(inout float s, out float pdf) {
    vec2 uv = Rand2(s);
    float z = sqrt(1.0 - uv.x);
    float phi = uv.y * TWO_PI;
    float sinTheta = sqrt(uv.x);
    vec3 dir = vec3(sinTheta * cos(phi), sinTheta * sin(phi), z);
    pdf = z * INV_PI;
    return dir;
}

void LocalBasis(vec3 n, out vec3 b1, out vec3 b2) {
    float sign_ = sign(n.z);
    if (n.z == 0.0) {
        sign_ = 1.0;
    }
    float a = -1.0 / (sign_ + n.z);
    float b = n.x * n.y * a;
    b1 = vec3(1.0 + sign_ * n.x * n.x * a, sign_ * b, -sign_ * n.x);
    b2 = vec3(b, sign_ + n.y * n.y * a, -n.y);
}

vec4 Project(vec4 a) {
    return a / a.w;
}

float GetDepth(vec3 posWorld) {
    float depth = (ubo2.world2clip * vec4(posWorld, 1.0)).w;
    return depth;
}

vec2 GetScreenCoordinate(vec3 posWorld) {
    vec2 uv = Project(ubo2.world2clip * vec4(posWorld, 1.0)).xy * 0.5 + 0.5;
    return uv;
}

float GetGBufferDepth(vec2 uv) {
    float depth = textureLod(gDepthSampler, uv, 0).x;
    if (depth < 1e-2) {
        depth = 1000.0;
    }
    return depth;
}

vec3 GetGBufferWorldNormal(vec2 uv) {
    // Stored as R8G8B8A8_SNORM, so quantisation leaves it slightly off unit length.
    vec3 normal = normalize(texture(gWorldNormalSampler, uv).xyz);
    return normal;
}

vec3 GetGBufferWorldPosition(vec2 uv) {
    vec3 posWorld = texture(gWorldPositionSampler, uv).xyz;
    return posWorld;
}

vec3 GetGBufferAlbedo(vec2 uv) {
    // gAlbedo is an sRGB format, the sampler already returns linear values.
    vec3 albedo = texture(gAlbedoSampler, uv).xyz;
    return albedo;
}

vec3 EvalDiffuse(vec3 wi, vec3 wo, vec2 uv) {
    vec3 normal = GetGBufferWorldNormal(uv);
    vec3 albedo = GetGBufferAlbedo(uv);
    vec3 bsdf = albedo / M_PI * max(dot(normal, wi), 0.0);
    return bsdf;
}

// Albedo of the shaded pixel is a constant factor of the whole indirect estimate, so it is
// left out here and multiplied back after denoising. Filtering the demodulated signal keeps
// surface detail perfectly sharp no matter how wide the spatial kernel gets.
float EvalDiffuseNoAlbedo(vec3 wi, vec2 uv) {
    vec3 normal = GetGBufferWorldNormal(uv);
    return max(dot(normal, wi), 0.0) * INV_PI;
}

float getBias(float ctrl, vec3 worldNormal)
{
    float m = 10.0 / 2048.0 / 2.0;
    float bias = max(m, m * (1.0 - dot(worldNormal, ubo2.lightDir))) * ctrl;
    return bias;
}

float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    return dot(rgbaDepth, bitShift);
}

float GetVisibility(vec2 uv){
    //return 1.0;
    vec3 worldNormal = GetGBufferWorldNormal(uv);
    vec3 worldPos = GetGBufferWorldPosition(uv);
    worldPos += worldNormal * 1e-2;
    vec4 lightSpaceCoord = vec4(ubo2.lightVP * vec4(worldPos, 1.0));
    vec3 NDC = lightSpaceCoord.xyz / lightSpaceCoord.w;
    NDC.xy = (NDC.xy + 1.0) * .5;
    float depth = unpack(texture(smSampler, NDC.xy));
    if(NDC.z <= depth + getBias(1.4, worldNormal))
        return 1.0;
    else
        return 0.0;
}

vec3 EvalDirectionalLight(vec2 uv) {
    return ubo2.lightRadiance * GetVisibility(uv);
}


#define MAX_MIP_LEVEL 9.0
#define MAX_ITERATIONS 128

bool RayMarchAcc(vec3 ori, vec3 dir, out vec3 hitPos) {
    dir = normalize(dir);

    vec3 currentOri = ori + dir * ACC_STEP_SIZE * 2.0;
    //vec3 currentOri = ori;
    float currentMip = 0.f;

    for(int i = 0; i < MAX_ITERATIONS && currentMip >= 0.0; ++i) {
        float stepDist = ACC_STEP_SIZE * exp2(currentMip);
        vec3 nextOri = currentOri + dir * stepDist;

        vec4 currentClipPos = ubo2.world2clip * vec4(nextOri, 1.f);
        if(currentClipPos.w <= 0.0) return false;

        float currentZ = currentClipPos.w;
        vec2 uv = currentClipPos.xy / currentClipPos.w;
        uv = (uv + vec2(1.f)) * vec2(.5f);
        if(uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
            // out of screen
            if(currentMip > 0.0){
                currentMip -= 1.0;
                continue;
            }else{
                return false;
            }
        }


        float recordZ = textureLod(gDepthSampler, uv, currentMip).r;
        if(currentZ > recordZ){
            if(currentMip > 0.0){
                currentMip -= 1.0;
            }else{
                if(currentZ - recordZ < 1.0) {
                   hitPos = texture(gWorldPositionSampler, uv).xyz;
                   return true;
                } else {
                    currentOri = nextOri;
                }
            }
        }else{
            currentOri = nextOri;
            currentMip = min(currentMip + 1.0, MAX_MIP_LEVEL);
        }
    }

    return false;
}

bool RayMarch(vec3 ori, vec3 dir, out vec3 hitPos) {
    dir = normalize(dir);
    for(int i = 0; i < 150; ++i){
        vec2 uv = GetScreenCoordinate(ori);
        float curDepth = GetDepth(ori);
        float recordDepth = GetGBufferDepth(uv);
        if(curDepth > recordDepth + 0.01){
            hitPos = ori;
            return true;
        }
        ori += dir * STEP_SIZE;
    }
    return false;
}

void main() {
    float s = InitRand(gl_FragCoord.xy + vec2(0.754877666, 0.569840296) * float(ubo2.temporalFrameIndex));
    vec2 uv = texCoords;

    float rawLinearDepth = textureLod(gDepthSampler, uv, 0).x;
    if(rawLinearDepth >= 99.f){ // zFar == 100.f
        vec4 background = vec4(0.f, 0.f, 0.f, 1.f);
        outColor = background;
        outHistory = background;
        outMoments = vec2(rawLinearDepth, 1.0);
        outDirectLight = background;
        return;
    }
    vec3 L_indir = vec3(0.0);
    vec3 b1, b2, worldNormal;
    worldNormal = GetGBufferWorldNormal(uv);
    LocalBasis(worldNormal, b1, b2);
    //mat3 tangent2world = mat3(worldNormal, b1, b2);
    mat3 tangent2world = mat3(b1, b2, worldNormal);
    vec3 worldPos = GetGBufferWorldPosition(uv);
    vec3 wo = normalize(ubo2.cameraPos - worldPos);
    for(int i = 0; i < SAMPLE_NUM; ++i){
        float pdf;
        vec3 sampleDir = vec3(tangent2world * SampleHemisphereCos(s, pdf));
        vec3 hitPos;
        if(dot(sampleDir, worldNormal) > 0.0 && RayMarchAcc(worldPos, sampleDir, hitPos)){
            vec3 wi = normalize(hitPos - worldPos);
            vec2 uvReflect = GetScreenCoordinate(hitPos);
            L_indir += (EvalDiffuseNoAlbedo(wi, uv) / pdf) * EvalDiffuse(normalize(-ubo2.lightDir), -wi, uvReflect) * EvalDirectionalLight(uvReflect);
        }
    }
    L_indir /= float(SAMPLE_NUM);
    vec3 L_dir = EvalDiffuse(normalize(-ubo2.lightDir), wo, uv) * EvalDirectionalLight(uv);
    vec3 indirect = clamp(L_indir, vec3(0.0), vec3(1.0));
    
    float sampleCount = 1.0;
    vec4 prevClip = ubo2.prevWorld2Clip * vec4(worldPos, 1.0);
    if (ubo2.historyValid > 0.0 && prevClip.w > 0.0) {
        vec2 prevUv = (prevClip.xy / prevClip.w) * 0.5 + 0.5;
        if (all(greaterThanEqual(prevUv, vec2(0.0))) && all(lessThanEqual(prevUv, vec2(1.0)))) {
            // A depth mismatch means something else occupied that pixel last frame, so this
            // surface was disoccluded and its history has to be dropped.
            vec2 prevMoments = texture(momentsSampler, prevUv).xy;
            if (abs(prevMoments.x - prevClip.w) <= HISTORY_DEPTH_TOLERANCE * prevClip.w) {
                sampleCount = min(prevMoments.y + 1.0, ubo2.maxAccumFrames);
                // 1/n running average that decays into a fixed exponential blend once capped.
                indirect = mix(indirect, texture(historySampler, prevUv).rgb, 1.0 - 1.0 / sampleCount);
            }
        }
    }

    // Undenoised composite, only kept so the swap chain attachment holds something meaningful.
    outColor = vec4(clamp(L_dir + GetGBufferAlbedo(uv) * indirect, vec3(0.0), vec3(1.0)), 1.0);
    outHistory = vec4(indirect, 1.0);
    outMoments = vec2((ubo2.world2clip * vec4(worldPos, 1.0)).w, sampleCount);
    outDirectLight = vec4(L_dir, 1.0);
}
