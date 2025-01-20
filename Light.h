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
#endif //VKRENDERINGENGINE_LIGHT_H
