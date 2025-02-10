#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

layout(location = 0) out vec2 TexCoords;
layout(location = 1) out vec3 WorldPos;
layout(location = 2) out vec3 Normal;

layout(binding = 0, std140) uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 normalMatrix;
}ubo;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(ubo.model * vec4(aPos, 1.0));
    Normal = (ubo.normalMatrix * vec4(aNormal, 1.0)).xyz;

    gl_Position =  ubo.proj * ubo.view * vec4(WorldPos, 1.0);
}