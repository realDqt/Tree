#version 450
layout(location = 0) in vec3 worldPosition;
layout(location = 1) in vec3 worldNormal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec4 clipPosition;

layout(location = 0) out vec4 gAlbedo;
layout(location = 1) out vec4 gWorldPosition;
layout(location = 2) out vec4 gWorldNormal;
layout(location = 3) out float gDepth;

layout(binding = 1) uniform sampler2D texSampler;

layout(push_constant, std140) uniform PushConstants{
    bool isFloor;
} constants;


void main() {
    gAlbedo = constants.isFloor ? vec4(1.0) : texture(texSampler, texCoords);
    gWorldPosition = vec4(worldPosition, 1.0);
    gWorldNormal = vec4(normalize(worldNormal), 0.0);
    gDepth = clipPosition.w;
}