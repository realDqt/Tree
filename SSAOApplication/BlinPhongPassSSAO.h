//
// Created by Administrator on 2025/10/15.
//

#ifndef VKRENDERINGENGINE_BLINPHONGPASSSSAO_H
#define VKRENDERINGENGINE_BLINPHONGPASSSSAO_H
#include "SSAOutils.h"

class BlinPhongPassSSAO{
public:
    struct alignas(16) UniformBufferObject{
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 modelInvTrans;
    };

    struct alignas(16) UniformBufferObject2{
        glm::vec3 cameraPos;
        alignas(16) glm::vec3 lightDir;
        alignas(16) glm::vec3 lightRadiance;
    };

    /*
    * device, physicalDevice
    * depthImageView
    * vertexBuffe, indexBuffer, indicesCount
    * currentFrame, swapChainImageViews, swapChainExtent, swapChainImageFormat
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
#endif //VKRENDERINGENGINE_BLINPHONGPASSSSAO_H
