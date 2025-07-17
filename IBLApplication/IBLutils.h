//
// Created by 22473 on 2025-02-10.
//

#ifndef VKRENDERINGENGINE_IBLUTILS_H
#define VKRENDERINGENGINE_IBLUTILS_H
#include "../EngineCore/VulkanTools.h"
constexpr int CUBEMAP_RESOLUTION = 512;
constexpr int IRRADIANCE_MAP_RESOLUTION = 32;
constexpr int MIPMAP_COUNT = 5;
constexpr int PREFILTER_MAP_RESOLUTION = 128;
constexpr int LUT_RESOLUTION = 512;
constexpr int NR_ROWS = 7;
constexpr int NR_COLUMNS = 7;
constexpr float SPACING = 2.5;

extern const std::string HDR_TEXTURE_PATH;
#endif //VKRENDERINGENGINE_IBLUTILS_H
