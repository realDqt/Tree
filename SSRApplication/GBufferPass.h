//
// Created by 22473 on 2025-03-08.
//

#ifndef VKRENDERINGENGINE_GBUFFERPASS_H
#define VKRENDERINGENGINE_GBUFFERPASS_H
#include "SSRutils.h"
#include "../Vertex.h"

class GBufferPass{
public:
    struct alignas(16) UniformBufferObject{
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 modelInvTrans;
    };

    struct PushConstants{
        alignas(4) bool isFloor;
    };

    /*
    * device, physicalDevice
    * colorImageView
    * depthImageView
    * vertexBufferCube, indexBufferCube, indicesCountCube
    * currentFrame, swapChainImageViews, swapChainExtent, msaaSamples, swapChainImageFormat
    * textureImageView, textureSampler
    * model
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
    VkExtent2D swapChainExtent;
    uint32_t swapChainImageViewCount;

    VkImageView depthImageView;

    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    uint32_t indicesCount;

    VkImageView textureImageView;
    VkSampler textureSampler;

    VkImageView gAlbedoView;
    VkImageView gWorldPositionView;
    VkImageView gWorldNormalView;
    VkImageView gDepthView;

    glm::mat4 model;
    uint32_t currentFrame;
    bool isFloor;

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
#endif //VKRENDERINGENGINE_GBUFFERPASS_H
