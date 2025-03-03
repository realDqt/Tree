#version 450
layout(location = 0) in vec3 aPos;

layout(location = 0) out vec3 localPos;

layout(binding = 0, std140) uniform UniformBufferObject{
    mat4 view;
    mat4 proj;
} ubo;

void main()
{
    localPos = aPos;
    gl_Position =  ubo.proj * ubo.view * vec4(localPos, 1.0);
    //gl_Position.y = -gl_Position.y;
}