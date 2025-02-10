#version 450

layout(location = 0) in vec2 TexCoords;
layout(location = 1) in vec3 WorldPos;
layout(location = 2) in vec3 Normal;

layout(location = 0) out vec4 FragColor;

layout(binding = 1, std140) uniform MaterialParameters{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
} mp;

layout(binding = 2, std140) uniform LightParameters{
    vec3 lightPositions[4];
    vec3 lightColors[4];
} lp;

layout(push_constant, std140) uniform PushConstants{
    vec3 cameraPos;
} constants;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
    vec3 N = Normal;
    vec3 V = normalize(constants.cameraPos - WorldPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, mp.albedo, mp.metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i){
        vec3 L = normalize(lp.lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lp.lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lp.lightColors[i] * attenuation;

        float NDF = DistributionGGX(N, H, mp.roughness);
        float G = GeometrySmith(N, V, L, mp.roughness);
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 Ks = F;
        vec3 Kd = vec3(1.0) - Ks;

        Kd *= 1.0 - mp.metallic;
        float NdotL = max(dot(N, L), 0.0);

        Lo += (Kd * mp.albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * mp.albedo * mp.ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}