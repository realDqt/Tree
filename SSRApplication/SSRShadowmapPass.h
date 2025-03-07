//
// Created by 22473 on 2025-03-07.
//

#ifndef VKRENDERINGENGINE_SSRSHADOWMAPPASS_H
#define VKRENDERINGENGINE_SSRSHADOWMAPPASS_H
#include "SSRutils.h"
#include "../Vertex.h"

class SSRShadowmapPass{
public:
    struct alignas(16) UniformBufferObject{
        glm::mat4 cubeModel;
        glm::mat4 floorModel;
        glm::mat4 lightVP;
    };

    struct PushConstants{
        alignas(4) bool isFloor;
    };

    /*
     * device, physicalDevice
     * colorImageView
     * depthImageView
     * vertexBufferMarry, indexBufferMarry, indicesCountMarry
     * vertexBufferFloor, indexBufferFloor, indicesCountFloor
     * currentFrame, swapChainImagesCount
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


    VkImageView colorImageView;
    VkImageView depthImageView;

    VkBuffer vertexBufferMarry;
    VkBuffer indexBufferMarry;
    uint32_t indicesCountMarry;

    VkBuffer vertexBufferFloor;
    VkBuffer indexBufferFloor;
    uint32_t indicesCountFloor;

    uint32_t  currentFrame;
    glm::mat4 cubeModel;
    glm::mat4 floorModel;


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
#endif //VKRENDERINGENGINE_SSRSHADOWMAPPASS_H
