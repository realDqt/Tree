//
// Created by frankqtdong on 2025/3/30.
//

#ifndef NATIVEBLOOMPASS_H
#define NATIVEBLOOMPASS_H
#include "FFTBloomUtils.h"
class NativeBloomPass
{
public:
    struct UniformBufferObject {
        alignas(8) glm::uvec2 texSize;
        alignas(4) float emissity;
        alignas(4) int R;
        alignas(4) int method;
        alignas(4) float lumThreshold;
    };
    /*
     * device, physicalDevice
     * hdrSamper, hdrImageView, kernelSamper, kernelImageView, bloomImageView
     * hdrTexSize, ratio, R
     */

    VkDevice device;
    VkPhysicalDevice physicalDevice;

    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet descriptorSet;

    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;

    VkSampler hdrSamper;
    VkImageView hdrImageView;

    VkSampler kernelSamper;
    VkImageView kernelImageView;

    VkImageView bloomImageView;

    VkBuffer uniformBuffer;
    VkDeviceMemory uniformBufferMemory;
    void* uniformBufferMapped;

    glm::uvec2 hdrTexSize;

    void createDescriptorPool();
    void createDescriptorSetLayout();
    void createDescriptorSet();

    void createUniformBuffer();
    void updateUniformBuffer();

    void createComputePipeline();
    void recordCommandBuffer(VkCommandBuffer commandBuffer);

    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code)const;

    void init();
    void cleanup();

};
#endif //NATIVEBLOOMPASS_H
