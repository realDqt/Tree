#version 450
layout(location = 0) in vec3 worldPosition;
layout(location = 1) in vec3 worldNormal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec4 lightSpaceCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2, std140) uniform UniformBufferObject2{
    vec3 cameraPos;
    vec3 lightDir;
    vec3 lightRadiance;
    bool isFloor;
} ubo2;

layout(binding = 3) uniform sampler2D smSampler;

float getBias(float ctrl)
{
    float m = 10.0 / 2048.0 / 2.0;
    float bias = max(m, m * (1.0 - dot(worldNormal, ubo2.lightDir))) * ctrl;
    return bias;
}

float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    return dot(rgbaDepth, bitShift);
}

float getVisibility()
{
    vec3 NDC = lightSpaceCoord.xyz / lightSpaceCoord.w;
    NDC.xy = (NDC.xy + 1.0) * .5;
    float depth = unpack(texture(smSampler, NDC.xy));
    if(NDC.z <= depth + getBias(2.4))
        return 1.0;
    else
        return 0.0;
}

vec3 blinnPhong(){
    vec3 albedo = vec3(1.0);
    if(!ubo2.isFloor)
        albedo = texture(texSampler, texCoords).rgb;
    albedo = pow(albedo, vec3(2.2));

    vec3 ambient = 0.05 * albedo;

    vec3 lightDir = -normalize(ubo2.lightDir);
    vec3 normal = normalize(worldNormal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * ubo2.lightRadiance * albedo;

    vec3 viewDir = normalize(ubo2.cameraPos - worldPosition);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 32.0);
    float ks = 0.0;
    vec3 specular = ks * ubo2.lightRadiance * spec;

    vec3 radiance = (ambient + diffuse + specular);
    vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));

    return phongColor;
}

void main() {
    outColor = vec4(getVisibility() * blinnPhong(), 1.0);
    //outColor = vec4(1.0, 1.0, 1.0, 1.0);
}