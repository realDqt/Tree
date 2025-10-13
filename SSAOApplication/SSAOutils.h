//
// Created by Administrator on 2025/8/7.
//

#ifndef VKRENDERINGENGINE_SSAOUTILS_H
#define VKRENDERINGENGINE_SSAOUTILS_H
#include "../EngineCore/utils.h"
#include "../EngineCore/Light.h"
#include "../EngineCore/Vertex.h"
#include "../EngineCore/RenderPass/RenderPass.h"
#include "../EngineCore/RenderPass/PresentPass.h"

struct NoiseTexture {
    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    VkSampler sampler;
};

extern const std::string ROBOT_PATH;

extern const VkFormat gAlbedoFormatSSAO;
extern const VkFormat gWorldPositionFormatSSAO;
extern const VkFormat gWorldNormalFormatSSAO;
extern const VkFormat gDepthFormatSSAO;
#endif //VKRENDERINGENGINE_SSAOUTILS_H
