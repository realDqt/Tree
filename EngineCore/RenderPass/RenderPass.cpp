//
// Created by Administrator on 2025/7/17.
//
#include "RenderPass.h"

VkShaderModule RenderPass::createShaderModule(const std::vector<char> &code) const {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device.value(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

bool RenderPass::IsValid() {
    if(!device.has_value()){
        std::cerr << "device is not initialized!" << std::endl;
        return false;
    }
    if(!physicalDevice.has_value()){
        std::cerr << "physicalDevice is not initialized!" << std::endl;
        return false;
    }
    if(!vertexBuffer.has_value()){
        std::cerr << "vertexBuffer is not initialized!" << std::endl;
        return false;
    }
    if(!indexBuffer.has_value()){
        std::cerr << "indexBuffer is not initialized!" << std::endl;
        return false;
    }
    if(!indicesCount.has_value()){
        std::cerr << "indicesCount is not initialized!" << std::endl;
        return false;
    }
    if(!currentFrame.has_value()){
        std::cerr << "currentFrame is not initialized!" << std::endl;
        return false;
    }
    if(!swapChainExtent.has_value()){
        std::cerr << "swapChainExtent is not initialized" << std::endl;
        return false;
    }
    if(!swapChainImageViewCount.has_value()){
        std::cerr << "swapChainImageViewCount is not initialized" << std::endl;
        return false;
    }
    if(!depthImageView.has_value()){
        std::cerr << "depthImageView is not initialized" << std::endl;
        return false;
    }
    return true;
}
