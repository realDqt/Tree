//
// Created by Administrator on 2025/10/14.
//

#ifndef VKRENDERINGENGINE_SSAOPASS_H
#define VKRENDERINGENGINE_SSAOPASS_H
#include "SSAOutils.h"

class SSAOPass : public PresentPass{
public:
    struct UniformBufferObject {
        glm::mat4 projection;
        glm::vec4 samples[64];
    };

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    void init() override;

    void createRenderPass() override;
    void createFramebuffers() override;
    void createGraphicsPipeline() override;

    void createDescriptorSetLayout() override;
    void createDescriptorPool() override;
    void createDescriptorSets() override;

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) override;
    bool IsValid() override;

    void cleanup() override;

    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);


    std::optional<VkImageView> gViewPositionView{std::nullopt};
    std::optional<VkSampler> gViewPositionSampler{std::nullopt};

    std::optional<VkImageView> gViewNormalView{std::nullopt};
    std::optional<VkSampler> gViewNormalSampler{std::nullopt};

    std::optional<VkImageView> gDepthView{std::nullopt};
    std::optional<VkSampler> gDepthSampler{std::nullopt};

    std::optional<VkImageView> texNoiseView{std::nullopt};
    std::optional<VkSampler> texNoiseSampler{std::nullopt};

    std::optional<VkImageView> occlusionView{std::nullopt};

};
#endif //VKRENDERINGENGINE_SSAOPASS_H
