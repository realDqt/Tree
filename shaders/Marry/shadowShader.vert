#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

layout(binding = 0, std140) uniform UniformBufferObject{
    mat4 lightMVP;
} ubo;

void main(){
    gl_Position = ubo.lightMVP * vec4(inPosition, 1.0);
}