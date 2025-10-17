#version 450
layout(location = 0) in vec3 viewPosition;
layout(location = 1) in vec3 viewNormal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec4 clipPosition;

layout(location = 0) out vec4 gAlbedo;
layout(location = 1) out vec4 gViewPosition;
layout(location = 2) out vec4 gViewNormal;
layout(location = 3) out float gDepth;



void main() {
    gAlbedo = vec4(0.95, 0.95, 0.95, 1.0);
    gViewPosition = vec4(viewPosition, 1.0);
    gViewNormal = vec4(normalize(viewNormal), 0.0);
    gDepth = clipPosition.w;
}