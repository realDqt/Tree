//
// Created by Administrator on 2025/7/17.
//

#ifndef VKRENDERINGENGINE_PRESENTPASS_H
#define VKRENDERINGENGINE_PRESENTPASS_H
#include "RenderPass.h"

class PresentPass : public RenderPass{
public:
    std::vector<VkFramebuffer> framebuffers;

    std::optional<std::vector<VkImageView>> swapChainImageViews{std::nullopt};
    std::optional<VkExtent2D> swapChainExtent{std::nullopt};

    std::optional<VkFormat> swapChainImageFormat{std::nullopt};
    std::optional<VkImageView> depthImageView{std::nullopt};

    bool IsValid() override;
};
#endif //VKRENDERINGENGINE_PRESENTPASS_H
