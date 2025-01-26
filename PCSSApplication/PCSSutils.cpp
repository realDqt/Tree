//
// Created by 22473 on 2025-01-26.
//
#include "PCSSutils.h"
const std::string MARRY_PATH = "E:/clion_proj/Tree/models/Marry.obj";
const std::string FLOOR_PATH = "E:/clion_proj/Tree/models/floor.obj";
const std::string TEXTURE_PATH = "E:/clion_proj/Tree/textures/MarryTexture.png";

// light's info
DirectionalLight light{
        .lightDir = glm::vec3(0.0f, -1.0f, -1.0f),
        .lightIntensity = glm::vec3(0.8f)
};
glm::vec3 lightPos = glm::vec3(0.0f, 8.0f, 9.0f);
//glm::vec3 lightUp = glm::vec3(-0.531192, 0.544641, -0.649);
glm::vec3 lightUp = glm::vec3(0.0f, 1.0f, 0.0f);

