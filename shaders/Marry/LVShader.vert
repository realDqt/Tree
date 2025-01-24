#version 450

layout(location = 0) in vec3 inPosition;

layout(binding = 0, std140) uniform UniformBufferObject{
    mat4 mvp;
    vec3 lightIntensity;
} ubo;

layout(location = 0) out vec3 lightColor;

void main()
{
    gl_Position = ubo.mvp * vec4(inPosition, 1.0f);
    lightColor = ubo.lightIntensity;
}
