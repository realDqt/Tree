#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

layout(location = 0) out vec2 texCoords;
void main() {
    gl_Position = vec4(aPos, 1.0);
    texCoords = aTexCoords;
}