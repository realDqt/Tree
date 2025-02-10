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

    mat4 rotView = mat4(mat3(ubo.view)); // remove translation from the view matrix
    vec4 clipPos = vec4(ubo.proj * rotView * vec4(localPos, 1.0));
    gl_Position = clipPos.xyww;
}