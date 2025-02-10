//
// Created by 22473 on 2025-01-27.
//

#ifndef VKRENDERINGENGINE_BLINPHONGPASS_H
#define VKRENDERINGENGINE_BLINPHONGPASS_H
#include "PCSSutils.h"
#include "../Vertex.h"

class BlinPhongPass{
public:
    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
        alignas(16) glm::mat4 modelInvTrans;
        alignas(16) glm::mat4 lightMVP;
    };

    struct PushConstants {
        alignas(4) glm::vec3 cameraPos;
        alignas(1) bool isFloor;
    };

    /*
     * device, physicalDevice
     * colorImageView
     * depthImageView
     * vertexBufferMarry, indexBufferMarry, indicesCountMarry
     * vertexBufferFloor, indexBufferFloor, indicesCountFloor
     * currentFrame, swapChainImagesCount, swapChainExtent, msaaSamples, swapChainImageFormat
     * textureImageView, textureSampler
     * SMcolorImageView, SMSampler
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

    VkBuffer vertexBufferMarry;
    VkBuffer indexBufferMarry;
    uint32_t indicesCountMarry;

    VkBuffer vertexBufferFloor;
    VkBuffer indexBufferFloor;
    uint32_t indicesCountFloor;

    uint32_t  currentFrame;

    VkImageView SMcolorImageView;
    VkSampler SMSampler;

    VkImageView textureImageView;
    VkSampler textureSampler;

    std::vector<VkBuffer> lightBuffers;
    std::vector<VkDeviceMemory> lightBuffersMemory;
    std::vector<void*> lightBuffersMapped;

    void init();

    void createRenderPass();
    void createFramebuffers();
    void createGraphicsPipeline();

    void createUniformBuffers();
    void createLightBuffers();

    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code)const;

    void cleanup();

    void updateUniformBuffer(uint32_t currentImage);
    void updateLightBuffer(uint32_t currentImage);

};
#endif //VKRENDERINGENGINE_BLINPHONGPASS_H
