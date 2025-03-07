#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

layout(binding = 0, std140) uniform UniformBufferObject{
    mat4 cubeModel;
    mat4 floorModel;
    mat4 lightVP;
} ubo;

layout(push_constant, std140) uniform PushConstants{
    bool isFloor;
} constants;

void main(){
    vec4 worldPosition = vec4(0.0);
    if(constants.isFloor)
        worldPosition = ubo.floorModel * vec4(inPosition, 1.0);
    else
        worldPosition = ubo.cubeModel * vec4(inPosition, 1.0);
    gl_Position = ubo.lightVP * worldPosition;
}