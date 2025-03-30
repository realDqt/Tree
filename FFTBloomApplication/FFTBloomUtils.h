//
// Created by 22473 on 2025-03-25.
//

#ifndef VKRENDERINGENGINE_FFTBLOOMUTILS_H
#define VKRENDERINGENGINE_FFTBLOOMUTILS_H
#include "../VulkanTools.h"
constexpr int FB_CUBEMAP_RESOLUTION = 1024;
constexpr int R = 15;
constexpr float ratio = 0.1f;

extern const std::string FB_HDR_TEXTURE_PATH;
extern const std::string KERNEL_PATH;
extern const VkFormat HDR_FORMAT;
#endif //VKRENDERINGENGINE_FFTBLOOMUTILS_H
