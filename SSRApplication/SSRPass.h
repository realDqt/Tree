//
// Created by 22473 on 2025-04-11.
//

#ifndef VKRENDERINGENGINE_SSRPASS_H
#define VKRENDERINGENGINE_SSRPASS_H
#include "SSRutils.h"
#include "../Vertex.h"

class SSRPass{
public:
    struct alignas(16) UniformBufferObject{
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    struct alignas(16) UniformBufferObject2{
        glm::vec3 cameraPos;
        alignas(16) glm::vec3 lightDir;
        alignas(16) glm::vec3 lightRadiance;
        alignas(16) glm::mat4 world2clip;
        glm::mat4 lightVP;
    };

    /*
    * device, physicalDevice
    * depthImageView
    * vertexBuffer, indexBuffer, indicesCount
    * currentFrame, swapChainImageViews, swapChainExtent, swapChainImageFormat
    * gAlbedoImageView, gAlbedoSampler
    * gWorldPositionImageView, gWorldPositionSampler
    * gWorldNormalImageView, gWorldNormalSampler
    * gDepthImageView, gDepthSampler
    * smImageView, smSampler
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

    std::vector<VkBuffer> uniformBuffers2;
    std::vector<VkDeviceMemory> uniformBuffersMemory2;
    std::vector<void*> uniformBuffersMapped2;

    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkImageView> swapChainImageViews;
    VkExtent2D swapChainExtent;

    VkFormat swapChainImageFormat;

    VkImageView depthImageView;

    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    uint32_t indicesCount;


    VkImageView gAlbedoImageView;
    VkSampler gAlbedoSampler;

    VkImageView gWorldPositionImageView;
    VkSampler gWorldPositionSampler;

    VkImageView gWorldNormalImageView;
    VkSampler gWorldNormalSampler;

    VkImageView gDepthImageView;
    VkSampler gDepthSampler;

    VkImageView smImageView;
    VkSampler smSampler;


    glm::mat4 model;
    uint32_t currentFrame;

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
#endif //VKRENDERINGENGINE_SSRPASS_H
