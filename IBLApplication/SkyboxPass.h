//
// Created by 22473 on 2025-02-12.
//

#ifndef VKRENDERINGENGINE_SKYBOXPASS_H
#define VKRENDERINGENGINE_SKYBOXPASS_H
#include "IBLutils.h"
#include "../Vertex.h"
class SkyboxPass{
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
     * envCubemapView, envSampler
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

    VkImageView colorImageView;
    VkImageView depthImageView;

    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    uint32_t indicesCount;

    uint32_t  currentFrame;

    VkImageView envCubemapView;
    VkSampler envSampler;

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
#endif //VKRENDERINGENGINE_SKYBOXPASS_H
