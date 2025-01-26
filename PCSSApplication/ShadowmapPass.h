//
// Created by 22473 on 2025-01-26.
//

#ifndef VKRENDERINGENGINE_SHADOWMAPPASS_H
#define VKRENDERINGENGINE_SHADOWMAPPASS_H
#include "PCSSutils.h"
#include "../Vertex.h"

class ShadowmapPass{
public:
    struct UniformBufferObject{
        alignas(16) glm::mat4 lightMVP;
    };

    /*
     * device, physicalDevice
     * colorImage, colorImageMemory, colorImageView
     * depthImage, depthImageMemory, depthImageView
     * vertexBufferMarry, indexBufferMarry, indicesCountMarry
     * vertexBufferFloor, indexBufferFloor, indicesCountFloor
     * currentFrame
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

    uint32_t swapChainImagesCount;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkBuffer vertexBufferMarry;
    VkBuffer indexBufferMarry;
    uint32_t indicesCountMarry;

    VkBuffer vertexBufferFloor;
    VkBuffer indexBufferFloor;
    uint32_t indicesCountFloor;

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

    VkShaderModule createShaderModule(const std::vector<char>& code)const;

    void updateUniformBuffer(uint32_t currentImage);

};
#endif //VKRENDERINGENGINE_SHADOWMAPPASS_H
