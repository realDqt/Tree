#ifndef VKRENDERINGENGINE_FXAAPASS_H
#define VKRENDERINGENGINE_FXAAPASS_H

#include "SSRutils.h"
#include "../EngineCore/Vertex.h"

class FXAAPass {
public:
    VkDevice device;
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkImageView> inputImageViews;
    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;
    VkBuffer vertexBuffer;
    VkSampler inputSampler;
    uint32_t currentFrame;

    void init();
    void createRenderPass();
    void createFramebuffers();
    void createGraphicsPipeline();
    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code) const;
    void cleanup();
};

#endif
