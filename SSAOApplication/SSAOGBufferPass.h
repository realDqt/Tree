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

    struct PushConstants{
        alignas(4) bool isFloor;
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

    std::optional<VkImageView> gAlbedoView{std::nullopt};
    std::optional<VkImageView> gViewPositionView{std::nullopt};
    std::optional<VkImageView> gViewNormalView{std::nullopt};
    std::optional<VkImageView> gDepthView{std::nullopt};

    std::optional<glm::mat4> model{std::nullopt};
};


#endif //VKRENDERINGENGINE_SSAOGBUFFERPASS_H
