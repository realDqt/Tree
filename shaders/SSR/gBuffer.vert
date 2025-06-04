#version 450
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

layout(location = 0) out vec3 worldPosition;
layout(location = 1) out vec3 worldNormal;
layout(location = 2) out vec2 texCoords;
layout(location = 3) out vec4 clipPosition;

layout(binding = 0, std140) uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 modelInvTrans;
} ubo;

void main() {
    worldPosition = (ubo.model * vec4(aPosition, 1.0)).xyz;
    worldNormal = (ubo.modelInvTrans * vec4(aNormal, 0.0)).xyz;
    texCoords = aTexCoords;
    clipPosition = ubo.proj * ubo.view * ubo.model * vec4(aPosition, 1.0);
    gl_Position = clipPosition;
}