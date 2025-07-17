//
// Created by Administrator on 2025/7/17.
//

#ifndef VKRENDERINGENGINE_RENDERPASS_H
#define VKRENDERINGENGINE_RENDERPASS_H
#include "../utils.h"

class RenderPass{
public:
    std::optional<VkDevice> device{std::nullopt};
    std::optional<VkPhysicalDevice> physicalDevice{std::nullopt};

    VkRenderPass renderPass;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    std::optional<VkBuffer> vertexBuffer{std::nullopt};
    std::optional<VkBuffer> indexBuffer{std::nullopt};
    std::optional<uint32_t> indicesCount{std::nullopt};

    std::optional<uint32_t> currentFrame{std::nullopt};

    virtual void init() = 0;

    virtual void createRenderPass() = 0;
    virtual void createFramebuffers() = 0;
    virtual void createGraphicsPipeline() = 0;

    virtual void createDescriptorSetLayout() = 0;
    virtual void createDescriptorPool() = 0;
    virtual void createDescriptorSets() = 0;

    virtual void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) = 0;
    virtual bool IsValid();

    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code)const;

    virtual void cleanup() = 0;
};
#endif //VKRENDERINGENGINE_RENDERPASS_H
