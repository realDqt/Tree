#version 450
layout(location = 0) in vec2 texCoords;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D gAlbedoSampler;
layout(binding = 1) uniform sampler2D gViewPositionSampler;
layout(binding = 2) uniform sampler2D gViewNormalSampler;
layout(binding = 3) uniform sampler2D blurredOcclusionSampler;


layout(binding = 4, std140) uniform UniformBufferObject{
    vec3 pointLightPos;
    vec3 lightRadiance;
    float linear;
    float quadratic;
} ubo;


vec3 blinnPhong(){
    vec3 albedo = texture(gAlbedoSampler, texCoords).rgb;
    if(dot(albedo, vec3(1.0)) < 0.001)
            discard;

    float occlusion = texture(blurredOcclusionSampler, texCoords).r;

    vec3 ambient = vec3(occlusion * 0.3);

    vec3 viewPos = texture(gViewPositionSampler, texCoords).xyz;

    vec3 viewLightDir = normalize(ubo.pointLightPos - viewPos);
    vec3 viewNormal = normalize(texture(gViewNormalSampler, texCoords).rgb);
    float diff = max(dot(viewLightDir, viewNormal), 0.0);
    vec3 diffuse = diff * ubo.lightRadiance * albedo;

    vec3 viewDir = normalize(-viewPos);
    vec3 halfDir = normalize(viewLightDir + viewDir);
    float spec = pow(max(dot(halfDir, viewNormal), 0.0), 8.0);
    float ks = 0.0;
    vec3 specular = ks * ubo.lightRadiance * spec;

    float frag2lightDistance = length(ubo.pointLightPos - viewPos);
    float attenuation = 1.0 / (1.0 + ubo.linear * frag2lightDistance + ubo.quadratic * frag2lightDistance * frag2lightDistance);

    vec3 radiance = (ambient + diffuse + specular) * attenuation;
    //vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));

    return phongColor;
}

void main() {
    outColor = vec4(blinnPhong(), 1.0);
}