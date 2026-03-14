//
// Created by Administrator on 2026/3/14.
//

#ifndef VKRENDERINGENGINE_GENERATEHIZPASS_H
#define VKRENDERINGENGINE_GENERATEHIZPASS_H

#include "SSRutils.h"
#include <vector>

class GenerateHizPass {
public:
    VkDevice device;
    VkPhysicalDevice physicalDevice;

    // --- 外部传入的 gDepth 资源 ---
    VkImage hizImage;                  // 直接传入 gDepth
    VkFormat hizFormat;                // 传入 gDepthFormat (通常为 R32_SFLOAT 或 R16_SFLOAT)
    uint32_t hizWidth;                 // swapChainExtent.width
    uint32_t hizHeight;                // swapChainExtent.height
    uint32_t mipLevels;                // 刚刚在外部计算的 hizMipLevels

    VkSampler hizSampler;
    std::vector<VkImageView> mipImageViews;

    // --- 描述符与管线 ---
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;

    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;

    // --- 方法 ---
    void createHizSampler();
    void createMipImageViews();

    void createDescriptorPool();
    void createDescriptorSetLayout();
    void createDescriptorSets();

    void createComputePipeline();
    void recordCommandBuffer(VkCommandBuffer commandBuffer);

    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code) const;

    void init();
    void cleanup();
};

#endif //VKRENDERINGENGINE_GENERATEHIZPASS_H