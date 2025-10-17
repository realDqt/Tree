//
// Created by 22473 on 2025-01-20.
//

#ifndef VKRENDERINGENGINE_LIGHT_H
#define VKRENDERINGENGINE_LIGHT_H
#include "utils.h"
struct DirectionalLight{
    alignas(16)glm::vec3 lightDir;
    alignas(16)glm::vec3 lightIntensity;
};

struct PointLight{
    alignas(16)glm::vec3 lightIntensity;
    alignas(16)glm::vec3 lightPos;
    alignas(4) float linear;
    alignas(4) float quadratic;
};
#endif //VKRENDERINGENGINE_LIGHT_H
