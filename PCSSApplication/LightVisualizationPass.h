//
// Created by 22473 on 2025-01-27.
//

#ifndef VKRENDERINGENGINE_LIGHTVISUALIZATIONPASS_H
#define VKRENDERINGENGINE_LIGHTVISUALIZATIONPASS_H
#include "PCSSutils.h"
#include "../EngineCore/Vertex.h"
class LightVisualizationPass{
public:
    struct alignas(16) UniformBufferObject{
        alignas(16) glm::mat4 MVP;
        alignas(16) glm::vec3 lightIntensity;
    };

    /*
     * device, physicalDevice
     * colorImageView
     * depthImageView
     * vertexBuffer, indexBuffer, indicesCount
     * currentFrame, swapChainExtent, swapChainImageFormat, msaaSamples, swapChainImageViews
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
#endif //VKRENDERINGENGINE_LIGHTVISUALIZATIONPASS_H
