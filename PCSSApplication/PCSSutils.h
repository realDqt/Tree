//
// Created by 22473 on 2025-01-26.
//

#ifndef VKRENDERINGENGINE_PCSSUTILS_H
#define VKRENDERINGENGINE_PCSSUTILS_H
#include "../utils.h"
#include "../Light.h"
extern const std::string MARRY_PATH;
extern const std::string FLOOR_PATH;
extern const std::string TEXTURE_PATH;
constexpr int SMResolution = 2048;
constexpr float OrthoRange = 10.0f;

// light's info
extern DirectionalLight light;
extern glm::vec3 lightPos;
extern glm::vec3 lightUp;
#endif //VKRENDERINGENGINE_PCSSUTILS_H
