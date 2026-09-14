//
// Created by 22473 on 2025-03-04.
//
#include "SSRutils.h"

const std::string CUBE_PATH = projectPath("models/SSRModel/cube2.gltf");
const std::string CUBE_TEXTURE_PATH = projectPath("models/SSRModel/checker.png");

DirectionalLight ssrLight{
        .lightDir = glm::vec3(-0.01f, -1.0f, 1.0f),
        .lightIntensity = glm::vec3(1.2f)
};
glm::vec3 ssrLightPos = glm::vec3(5.0f, 5.0f, -5.0f);
glm::vec3 ssrLightUp = glm::vec3(0.0f, 1.0f, 0.0f);

const VkFormat gAlbedoFormat = VK_FORMAT_R8G8B8A8_SRGB;
const VkFormat gWorldPositionFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
const VkFormat gWorldNormalFormat = VK_FORMAT_R8G8B8A8_SNORM;
const VkFormat gDepthFormat = VK_FORMAT_R32_SFLOAT;
// Depth is manually packed into 4 bytes, so the storage must not apply any sRGB transfer curve.
const VkFormat shadowmapFormat = VK_FORMAT_R8G8B8A8_UNORM;
// Temporal accumulation happens in linear space and needs more than 8 bits per channel.
const VkFormat historyFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
// Per pixel reprojection state: view depth at the time of the write, and accumulated sample count.
// Half floats are enough because the depth tolerance is relative, and they guarantee linear filtering.
const VkFormat momentsFormat = VK_FORMAT_R16G16_SFLOAT;
// Direct lighting is noise free and bypasses the denoiser, so it is kept out of the history.
const VkFormat directLightFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
// Denoised and recomposited image, still linear because FXAA runs after it.
const VkFormat resolvedFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
