//
// Created by 22473 on 2025-03-04.
//

#ifndef VKRENDERINGENGINE_SSRUTILS_H
#define VKRENDERINGENGINE_SSRUTILS_H
#include "../utils.h"
#include "../Light.h"
extern const std::string CUBE_PATH;
extern const std::string CUBE_TEXTURE_PATH;

extern DirectionalLight ssrLight;
extern glm::vec3 ssrLightPos;
extern glm::vec3 ssrLightUp;

constexpr int SM_RESOLUTION = 2048;
constexpr float SSROrthoRange = 20.0f;
#endif //VKRENDERINGENGINE_SSRUTILS_H
