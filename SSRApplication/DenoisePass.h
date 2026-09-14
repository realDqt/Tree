//
// Joint bilateral denoiser for the screen space indirect term.
//
// Runs between the SSR pass and FXAA. It filters only the temporally accumulated, albedo
// demodulated indirect lighting, using the G-Buffer to stop the kernel at geometric edges,
// then recomposites albedo and the untouched direct lighting.
//

#ifndef VKRENDERINGENGINE_DENOISEPASS_H
#define VKRENDERINGENGINE_DENOISEPASS_H

#include "SSRutils.h"
#include "../EngineCore/Vertex.h"

class DenoisePass {
public:
    struct PushConstants {
        float maxAccumFrames;
        float phiNormal;
        float phiPlane;
    };

    VkDevice device;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> framebuffers;

    VkExtent2D swapChainExtent;
    VkBuffer vertexBuffer;

    // Per frame in flight, must line up with the rings the SSR pass writes.
    std::vector<VkImageView> indirectImageViews;
    std::vector<VkImageView> momentsImageViews;
    std::vector<VkImageView> directLightImageViews;
    std::vector<VkImageView> resolvedImageViews;
    VkSampler accumulationSampler;

    VkImageView gWorldPositionImageView;
    VkSampler gWorldPositionSampler;
    VkImageView gWorldNormalImageView;
    VkSampler gWorldNormalSampler;
    VkImageView gDepthImageView;
    VkSampler gDepthSampler;
    VkImageView gAlbedoImageView;
    VkSampler gAlbedoSampler;

    // Tight defaults: the kernel is meant to clean up indirect noise, not to soften geometry.
    float phiNormal = 128.0f;
    float phiPlane = 0.1f;
    float maxAccumFrames = 32.0f;

    uint32_t currentFrame;

    void init();
    void createRenderPass();
    void createFramebuffers();
    void createGraphicsPipeline();
    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();
    void recordCommandBuffer(VkCommandBuffer commandBuffer);
    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code) const;
    void cleanup();
};

#endif //VKRENDERINGENGINE_DENOISEPASS_H
