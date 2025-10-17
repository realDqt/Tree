//
// Created by Administrator on 2025/10/17.
//

#ifndef VKRENDERINGENGINE_BLURPASS_H
#define VKRENDERINGENGINE_BLURPASS_H
#include "SSAOutils.h"

class BlurPass : public RenderPass{
public:

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

    std::optional<VkImageView> occlusionView{std::nullopt};
    std::optional<VkSampler> occlusionSampler{std::nullopt};

    std::optional<VkImageView> blurredOcclusionView{std::nullopt};
};
#endif //VKRENDERINGENGINE_BLURPASS_H
