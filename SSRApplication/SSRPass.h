//
// Created by 22473 on 2025-04-11.
//

#ifndef VKRENDERINGENGINE_SSRPASS_H
#define VKRENDERINGENGINE_SSRPASS_H
#include "SSRutils.h"
#include "../EngineCore/Vertex.h"

class SSRPass{
public:
    struct alignas(16) UniformBufferObject2{
        glm::vec3 cameraPos;
        alignas(16) glm::vec3 lightDir;
        alignas(16) glm::vec3 lightRadiance;
        alignas(16) glm::mat4 world2clip;
        glm::mat4 lightVP;
        glm::mat4 prevWorld2Clip;
        alignas(16) uint32_t temporalFrameIndex;
        float historyValid;
        float maxAccumFrames;
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

    std::vector<VkBuffer> uniformBuffers2;
    std::vector<VkDeviceMemory> uniformBuffersMemory2;
    std::vector<void*> uniformBuffersMapped2;

    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkImageView> swapChainImageViews;
    VkExtent2D swapChainExtent;

    VkFormat swapChainImageFormat;

    VkImageView depthImageView;

    VkBuffer vertexBuffer;



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

    std::vector<VkImageView> historyImageViews;
    VkSampler historySampler;

    // Reprojection state: previous frame view depth and sample count, sampled with historySampler.
    std::vector<VkImageView> momentsImageViews;

    glm::mat4 previousWorld2Clip{1.0f};
    bool historyValid = false;
    uint32_t temporalFrameIndex = 0;

    // Caps the exponential moving average once the ramp 1/n reaches 1/maxAccumFrames.
    float maxAccumFrames = 32.0f;

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
