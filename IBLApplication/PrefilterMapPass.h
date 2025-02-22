//
// Created by 22473 on 2025-02-21.
//

#ifndef VKRENDERINGENGINE_PREFILTERMAPPASS_H
#define VKRENDERINGENGINE_PREFILTERMAPPASS_H
#include "IBLutils.h"
#include "../Vertex.h"
class PrefilterMapPass{
public:
    struct alignas(16) UniformBufferObject{
        glm::mat4 view;
        glm::mat4 proj;
    };

    struct alignas(4) PushConstants{
        float roughness;
    };

    /*
     * device, physicalDevice
     * colorImageView
     * depthImageView
     * vertexBuffer, indexBuffer, indicesCount
     * envImageView, envSampler
     * viewMat, colorImageExtent
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


    VkImageView envImageView;
    VkSampler envSampler;

    VkImageView colorImageView;
    VkImageView depthImageView;

    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    uint32_t indicesCount;

    VkExtent2D colorImageExtent;
    float roughness;

    glm::mat4 viewMat;

    void init();

    void createRenderPass();
    void createFramebuffers();
    void createGraphicsPipeline();

    void createUniformBuffers();

    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();

    void recordCommandBuffer(VkCommandBuffer commandBuffer);

    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code)const;

    void cleanup();

    void updateUniformBuffer();
};
#endif //VKRENDERINGENGINE_PREFILTERMAPPASS_H
