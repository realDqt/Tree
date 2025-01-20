#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec3 fragPos;
layout(location = 2) out vec3 worldNormal;

layout(binding = 0, std140) uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 modelInvTrans;
} ubo;

void main()
{
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
    fragPos = (ubo.model * vec4(inPosition, 1.0)).xyz;
    worldNormal = (ubo.modelInvTrans * vec4(inNormal, 0.0)).xyz;
}