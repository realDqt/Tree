//
// Created by Administrator on 2025/10/15.
//

#ifndef VKRENDERINGENGINE_DEFERREDLIGHTPASS_H
#define VKRENDERINGENGINE_DEFERREDLIGHTPASS_H
#include "SSAOutils.h"

class DeferredLightPass : public PresentPass{
public:
    struct alignas(16) UniformBufferObject{
        glm::vec3 pointLightPos;
        alignas(16) glm::vec3 lightRadiance;
        alignas(4) float linear;
        alignas(4) float quadratic;
    };



    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    std::optional<VkImageView> gAlbedoView;
    std::optional<VkSampler> gAlbedoSampler;

    std::optional<VkImageView> gViewPositionView;
    std::optional<VkSampler> gViewPositionSampler;

    std::optional<VkImageView> gViewNormalView;
    std::optional<VkSampler> gViewNormalSampler;

    std::optional<VkImageView> blurredOcclusionView;
    std::optional<VkSampler> blurredOcclusionSampler;

    void init() override;

    void createRenderPass() override;
    void createFramebuffers() override;
    void createGraphicsPipeline() override;

    void createUniformBuffers();

    void createDescriptorSetLayout() override;
    void createDescriptorPool() override;
    void createDescriptorSets() override;

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) override;
    bool IsValid() override;

    void cleanup() override;

    void updateUniformBuffer(uint32_t currentImage);

};
#endif //VKRENDERINGENGINE_DEFERREDLIGHTPASS_H
