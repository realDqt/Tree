//
// Created by 22473 on 2025-02-13.
//

#ifndef VKRENDERINGENGINE_SPHEREPASS_H
#define VKRENDERINGENGINE_SPHEREPASS_H
#include "IBLutils.h"
#include "../Vertex.h"
class SpherePass{
public:
    struct alignas(16) UniformBufferObject{
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
        alignas(16) glm::mat4 normalMatrix;
    };

    struct alignas(16) MaterialParameters {
        alignas(16) glm::vec3 albedo;  // vec3 转为 vec4，16 字节对齐
        float metallic;
        float roughness;
        float ao;
    };

    struct alignas(16) LightParameters {
        alignas(16) glm::vec3 lightPositions[4];  // vec3 转为 vec4，16 字节对齐
        alignas(16) glm::vec3 lightColors[4];     // vec3 转为 vec4，16 字节对齐
    };

    struct alignas(16) PushConstants{
        alignas(16) glm::vec3 cameraPos;
    };

    struct ExternalInfo{
        glm::mat4 model;
        glm::mat4 normalMatrix;
        glm::vec3 albedo;
        float metallic;
        float roughness;
        float ao;
    };



    /*
     * device, physicalDevice
     * colorImageView
     * depthImageView
     * vertexBuffer, indexBuffer, indicesCount
     * currentFrame, swapChainExtent, swapChainImageFormat, msaaSamples, swapChainImageViews
     * externalInfo
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

    std::vector<VkBuffer> materialParameters;
    std::vector<VkDeviceMemory> materialParametersMemory;
    std::vector<void*> materialParametersMapped;

    std::vector<VkBuffer> lightParameters;
    std::vector<VkDeviceMemory> lightParametersMemory;
    std::vector<void*> lightParametersMapped;


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
    ExternalInfo externalInfo;


    void init();

    void createRenderPass();
    void createFramebuffers();
    void createGraphicsPipeline();

    void createUniformBuffers();
    void createMaterialParameters();
    void createLightParameters();

    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code)const;

    void cleanup();

    void updateUniformBuffer(uint32_t currentImage);
    void updateMaterialParameters(uint32_t currentImage);
    void updateLightParameters(uint32_t currentImage);
};
#endif //VKRENDERINGENGINE_SPHEREPASS_H
