//
// Created by 22473 on 2025-02-18.
//

#ifndef VKRENDERINGENGINE_IRRADIANCEMAPPASS_H
#define VKRENDERINGENGINE_IRRADIANCEMAPPASS_H
#include "IBLutils.h"
#include "../EngineCore/Vertex.h"
class IrradianceMapPass{
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
     * envImageView, envSampler
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


    VkImageView envImageView;
    VkSampler envSampler;

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
#endif //VKRENDERINGENGINE_IRRADIANCEMAPPASS_H
