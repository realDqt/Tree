#version 450
layout(location = 0) in vec2 texCoords;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D gViewPositionSampler;
layout(binding = 1) uniform sampler2D gViewNormalSampler;
layout(binding = 2) uniform sampler2D blurredOcclusionSampler;


layout(binding = 3, std140) uniform UniformBufferObject{
    vec3 cameraPos;
    vec3 viewLightDir;
    vec3 lightRadiance;
} ubo;


vec3 blinnPhong(){
    vec3 albedo = vec3(0.3);
    float occlusion = texture(blurredOcclusionSampler, texCoords).r;

    vec3 ambient = albedo * occlusion;

    vec3 viewLightDir = -normalize(ubo.viewLightDir);
    vec3 viewNormal = normalize(texture(gViewNormalSampler, texCoords).rgb);
    float diff = max(dot(viewLightDir, viewNormal), 0.0);
    vec3 diffuse = diff * ubo.lightRadiance * albedo;

    vec3 viewDir = normalize(-texture(gViewPositionSampler, texCoords).xyz);
    vec3 halfDir = normalize(viewLightDir + viewDir);
    float spec = pow(max(dot(halfDir, viewNormal), 0.0), 8.0);
    float ks = 0.0;
    vec3 specular = ks * ubo.lightRadiance * spec;

    vec3 radiance = (ambient + diffuse + specular);
    vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));

    return phongColor;
}

void main() {
    outColor = vec4(blinnPhong(), 1.0);
    //outColor = vec4(1.0, 1.0, 1.0, 1.0);
}