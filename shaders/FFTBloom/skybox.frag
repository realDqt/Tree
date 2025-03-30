#version 450
layout(location = 0) in vec3 localPos;

layout(location = 0) out vec4 FragColor;

layout(binding = 1) uniform samplerCube environmentMap;

// 3. ACES色调映射
vec3 ACES(vec3 color) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0, 1.0);
}

void main()
{
    vec3 envColor = texture(environmentMap, localPos).rgb;

    //envColor = envColor / (envColor + vec3(1.0));
    //envColor = ACES(envColor);
    //envColor = pow(envColor, vec3(1.0/2.2));

    FragColor = vec4(envColor, 1.0);
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}