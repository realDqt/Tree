//
// Created by Administrator on 2025/8/7.
//

#ifndef VKRENDERINGENGINE_SSAOGBUFFERPASS_H
#define VKRENDERINGENGINE_SSAOGBUFFERPASS_H
#include "SSAOutils.h"

class SSAOGBufferPass : public RenderPass{
public:
    struct alignas(16) UniformBufferObject{
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 modelInvTrans;
    };

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

    VkImageView gAlbedoView;
    VkImageView gWorldPositionView;
    VkImageView gWorldNormalView;
    VkImageView gDepthView;

    glm::mat4 model;
};


#endif //VKRENDERINGENGINE_SSAOGBUFFERPASS_H
