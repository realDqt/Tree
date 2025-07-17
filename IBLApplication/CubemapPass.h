//
// Created by 22473 on 2025-02-12.
//

#ifndef VKRENDERINGENGINE_CUBEMAPPASS_H
#define VKRENDERINGENGINE_CUBEMAPPASS_H
#include "IBLutils.h"
#include "../EngineCore/Vertex.h"
class CubemapPass{
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
     * hdrImageView, hdrSampler
     * viewMat
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


    VkImageView hdrImageView;
    VkSampler hdrSampler;

    VkImageView colorImageView;
    VkImageView depthImageView;

    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    uint32_t indicesCount;

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
#endif //VKRENDERINGENGINE_CUBEMAPPASS_H
