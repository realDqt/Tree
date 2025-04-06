#version 450
layout(location = 0) in vec3 localPos;

layout(location = 0) out vec4 FragColor;

layout(binding = 1) uniform samplerCube environmentMap;

vec3 ACESToneMapping(vec3 color, float adapted_lum)
{
    const float A = 2.51f;
    const float B = 0.03f;
    const float C = 2.43f;
    const float D = 0.59f;
    const float E = 0.14f;

    color *= adapted_lum;
    return (color * (A * color + B)) / (color * (C * color + D) + E);
}

vec3 ReinhardToneMapping(vec3 color){
    return color / (color + vec3(1.0));
}

void main()
{
    vec3 envColor = texture(environmentMap, localPos).rgb;

    //envColor = envColor / (envColor + vec3(1.0));
    envColor = ReinhardToneMapping(envColor);
    envColor = pow(envColor, vec3(1.0/2.2));

    FragColor = vec4(envColor, 1.0);
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}