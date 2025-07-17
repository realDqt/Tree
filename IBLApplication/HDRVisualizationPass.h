//
// Created by 22473 on 2025-02-10.
//

#ifndef VKRENDERINGENGINE_HDRVISUALIZATIONPASS_H
#define VKRENDERINGENGINE_HDRVISUALIZATIONPASS_H
#include "IBLutils.h"
#include "../EngineCore/Vertex.h"
class HDRVisualizationPass{
public:
    struct UniformBufferObject{
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    /*
     * device, physicalDevice
     * colorImageView
     * depthImageView
     * vertexBuffer, indexBuffer, indicesCount
     * currentFrame, swapChainExtent, swapChainImageFormat, msaaSamples, swapChainImageViews
     * hdrImageView, hdrSampler
     */

    VkDevice device;
    VkPhysicalDevice physicalDevice;

    VkRenderPass renderPass;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkImageView> swapChainImageViews;
    VkExtent2D swapChainExtent;

    VkFormat swapChainImageFormat;
    VkSampleCountFlagBits msaaSamples;

    VkImageView hdrImageView;
    VkSampler hdrSampler;

    VkImageView colorImageView;
    VkImageView depthImageView;

    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    uint32_t indicesCount;

    uint32_t  currentFrame;

    glm::mat4 viewMatTest;

    void init();

    void createRenderPass();
    void createFramebuffers();
    void createGraphicsPipeline();

    void createUniformBuffers();

    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code)const;

    void cleanup();

    void updateUniformBuffer(uint32_t currentImage);
};
#endif //VKRENDERINGENGINE_HDRVISUALIZATIONPASS_H
