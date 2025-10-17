//
// Created by Administrator on 2025/8/7.
//
#include "SSAOutils.h"

const std::string ROBOT_PATH = "E:\\Proj\\ClionProj\\Tree\\models\\SSAOModel\\nanosuit.obj";
const std::string MARRY_SSAO_PATH = "E:\\Proj\\ClionProj\\Tree\\models\\Marry.obj";

PointLight ssaoLight{
        .lightIntensity = glm::vec3(0.1, 0.1, 0.5),
        .lightPos = glm::vec3(2.0, 4.0, -2.0),
        .linear = 0.09,
        .quadratic = 0.032,
};

const VkFormat gAlbedoFormatSSAO = VK_FORMAT_R8G8B8A8_SRGB;
const VkFormat gWorldPositionFormatSSAO = VK_FORMAT_R32G32B32A32_SFLOAT;
const VkFormat gWorldNormalFormatSSAO = VK_FORMAT_R8G8B8A8_SNORM;
const VkFormat gDepthFormatSSAO = VK_FORMAT_R32_SFLOAT;

const VkFormat occlusionFormat = VK_FORMAT_R32_SFLOAT;

