#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 worldNormal;
layout(location = 3) in vec4 lightSpaceCoord;


layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2, std140) uniform DirectionalLight{
    vec3 lightDir;
    vec3 lightIntensity;
} light;

layout(push_constant, std140)uniform PushConstants{
    vec3 cameraPos;
    bool isFloor;
} constants;

layout(binding = 3) uniform sampler2D smSampler;

// Shadow map related variables
#define NUM_SAMPLES 20
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10

#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586

vec3 PhongColor()
{
    vec3 albedo = vec3(0.0);
    vec3 uKs = vec3(0.0);

    if(constants.isFloor){
        albedo = vec3(0.8);
    }else{
        albedo = texture(texSampler, fragTexCoord).rgb;
    }


    vec3 ambient = 0.05 * albedo;

    float diff = max(0, dot(light.lightDir, worldNormal));
    vec3 diffuse = diff * albedo * light.lightIntensity;

    vec3 viewDir = constants.cameraPos - fragPos;
    vec3 halfVec = normalize(viewDir + light.lightDir);
    float spec = pow(max(0, dot(halfVec, worldNormal)), 32.f);
    vec3 specular = uKs * spec * light.lightIntensity;

    return pow(ambient + diffuse + specular, vec3(1.0/2.2));
}

float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    return dot(rgbaDepth, bitShift);
}

float GetVisbility()
{
    // calculate visbility
    vec3 NDC = lightSpaceCoord.xyz / lightSpaceCoord.w;
    NDC.xy = (NDC.xy + 1.0f) * .5f;
    float depth = unpack(texture(smSampler, NDC.xy));
    float shadingDepth = NDC.z;
    float bias = max(0.04f * (1.0f - dot(worldNormal, light.lightDir)), 0.05) * 0.035;
    return shadingDepth <= depth + bias ? 1.0f : 0.0f;
}

void main()
{
    vec3 phongColor = PhongColor();

    float visbility = GetVisbility();

    outColor = vec4(visbility * phongColor, 1.0);
}