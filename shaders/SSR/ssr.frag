#version 450
layout(location = 0) in vec3 worldPosition;

layout(location = 0) out vec4 outColor;

layout(binding = 1, std140) uniform UniformBufferObject2{
    vec3 cameraPos;
    vec3 lightDir;
    vec3 lightRadiance;
    mat4 world2clip;
    mat4 lightVP;
} ubo2;

#define M_PI 3.1415926535897932384626433832795
#define TWO_PI 6.283185307
#define INV_PI 0.31830988618
#define INV_TWO_PI 0.15915494309

layout(binding = 2) uniform sampler2D gAlbedoSampler;
layout(binding = 3) uniform sampler2D gWorldPositionSampler;
layout(binding = 4) uniform sampler2D gWorldNormalSampler;
layout(binding = 5) uniform sampler2D gDepthSampler;
layout(binding = 6) uniform sampler2D smSampler;

float Rand1(inout float p) {
    p = fract(p * .1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

vec2 Rand2(inout float p) {
    return vec2(Rand1(p), Rand1(p));
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

/*
 * Transform point from world space to screen space([0, 1] x [0, 1])
 *
 */
vec2 GetScreenCoordinate(vec3 posWorld) {
    vec2 uv = Project(ubo2.world2clip * vec4(posWorld, 1.0)).xy * 0.5 + 0.5;
    return uv;
}

float GetGBufferDepth(vec2 uv) {
    float depth = texture(gDepthSampler, uv).x;
    if (depth < 1e-2) {
        depth = 1000.0;
    }
    return depth;
}

vec3 GetGBufferWorldNormal(vec2 uv) {
    vec3 normal = texture(gWorldNormalSampler, uv).xyz;
    return normal;
}

vec3 GetGBufferWorldPosition(vec2 uv) {
    vec3 posWorld = texture(gWorldPositionSampler, uv).xyz;
    return posWorld;
}

vec3 GetGBufferAlbedo(vec2 uv) {
    vec3 albedo = texture(gAlbedoSampler, uv).xyz;
    albedo = pow(albedo, vec3(2.2));
    return albedo;
}

/*
 * Evaluate diffuse bsdf value.
 *
 * wi, wo are all in world space.
 * uv is in screen space, [0, 1] x [0, 1].
 *
 */
vec3 EvalDiffuse(vec3 wi, vec3 wo, vec2 uv) {
    vec3 normal = normalize(GetGBufferWorldNormal(uv));
    vec3 albedo = GetGBufferAlbedo(uv);
    vec3 bsdf = albedo / M_PI * max(dot(normal, wi), 0.0);
    return bsdf;
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
    vec3 worldNormal = GetGBufferWorldNormal(uv);
    vec3 worldPos = GetGBufferWorldPosition(uv);
    vec4 lightSpaceCoord = vec4(ubo2.lightVP * vec4(worldPos, 1.0));
    vec3 NDC = lightSpaceCoord.xyz / lightSpaceCoord.w;
    NDC.xy = (NDC.xy + 1.0) * .5;
    float depth = unpack(texture(smSampler, NDC.xy));
    if(NDC.z <= depth + getBias(6.4, worldNormal))
        return 1.0;
    else
        return 0.0;
}

/*
 * Evaluate directional light with shadow map
 * uv is in screen space, [0, 1] x [0, 1].
 *
 */
vec3 EvalDirectionalLight(vec2 uv) {
    return ubo2.lightRadiance * GetVisibility(uv);
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
        ori += dir;
    }
    return false;
}

#define SAMPLE_NUM 1

void main() {
    float s = InitRand(gl_FragCoord.xy);
    vec2 uv = GetScreenCoordinate(worldPosition);
    vec3 L_indir = vec3(0.0);
    vec3 b1, b2, worldNormal;
    worldNormal = GetGBufferWorldNormal(uv);
    LocalBasis(worldNormal, b1, b2);
    mat3 tangent2world = mat3(worldNormal, b1, b2);
    vec3 wo = normalize(ubo2.cameraPos - worldPosition);
    vec3 worldPos = GetGBufferWorldPosition(uv);
    int cnt = 0;
    for(int i = 0; i < SAMPLE_NUM; ++i){
        float pdf;
        vec3 sampleDir = vec3(tangent2world * SampleHemisphereCos(s, pdf));
        vec3 hitPos;
        if(dot(sampleDir, worldNormal) > 0.0 && RayMarch(worldPos, sampleDir, hitPos)){
            vec3 wi = normalize(hitPos - worldPos);
            vec2 uvReflect = GetScreenCoordinate(hitPos);
            L_indir += (EvalDiffuse(wi, wo, uv) / pdf) * EvalDiffuse(ubo2.lightDir, -wi, uvReflect) * EvalDirectionalLight(uvReflect);
            ++cnt;
        }
    }
    L_indir /= (float(cnt) + 1e-3);
    vec3 L_dir = EvalDiffuse(ubo2.lightDir, wo, uv) * EvalDirectionalLight(uv);
    vec3 color = pow(clamp(L_dir + L_indir, vec3(0.0), vec3(1.0)), vec3(1.0 / 2.2));
    outColor = vec4(color, 1.0);
}