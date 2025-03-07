//
// Created by 22473 on 2025-03-04.
//
#include "SSRutils.h"

const std::string CUBE_PATH = "E:/clion_proj/Tree/models/SSRModel/cube2.gltf";
const std::string CUBE_TEXTURE_PATH = "E:/clion_proj/Tree/models/SSRModel/checker.png";

DirectionalLight ssrLight{
        .lightDir = glm::vec3(-0.01f, -1.0f, 1.0f),
        .lightIntensity = glm::vec3(0.2f)
};
glm::vec3 ssrLightPos = glm::vec3(5.0f, 5.0f, -5.0f);
glm::vec3 ssrLightUp = glm::vec3(0.0f, 1.0f, 0.0f);