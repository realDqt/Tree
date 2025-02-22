//
// Created by 22473 on 2025-02-22.
//

#ifndef VKRENDERINGENGINE_BRDFLUTPASS_H
#define VKRENDERINGENGINE_BRDFLUTPASS_H
#include "IBLutils.h"
#include "../Vertex.h"
class BrdfLUTPass{
public:

    /*
     * device, physicalDevice
     * colorImageView
     * depthImageView
     * vertexBuffer
     */

    VkDevice device;
    VkPhysicalDevice physicalDevice;

    VkRenderPass renderPass;

    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    std::vector<VkFramebuffer> framebuffers;

    VkImageView colorImageView;
    VkImageView depthImageView;

    VkBuffer vertexBuffer;

    void init();

    void createRenderPass();
    void createFramebuffers();
    void createGraphicsPipeline();

    void recordCommandBuffer(VkCommandBuffer commandBuffer);

    [[nodiscard]] VkShaderModule createShaderModule(const std::vector<char>& code)const;

    void cleanup();
};
#endif //VKRENDERINGENGINE_BRDFLUTPASS_H
