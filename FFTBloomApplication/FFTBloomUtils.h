//
// Created by 22473 on 2025-03-25.
//

#ifndef VKRENDERINGENGINE_FFTBLOOMUTILS_H
#define VKRENDERINGENGINE_FFTBLOOMUTILS_H
#include "../EngineCore/VulkanTools.h"
constexpr int FB_CUBEMAP_RESOLUTION = 1024;
constexpr int R = 20;                // 滤波半径
constexpr float emissity = 20.f;      // 发散程度
constexpr int methd = 0;             // 计算权重的方法（0： D5 kernel, 1： Gaussion, 2: Light Weighted Gaussion)
constexpr float lumThreshold = 30.f; // 光强阈值，超过此值被认为是高亮像素
constexpr bool output = true;


extern const std::string FB_HDR_TEXTURE_PATH;
extern const std::string KERNEL_PATH;
extern const std::string SAVE_PATH;
extern const VkFormat HDR_FORMAT;
#endif //VKRENDERINGENGINE_FFTBLOOMUTILS_H
