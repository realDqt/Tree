//
// Created by Administrator on 2025/7/17.
//
#include "PresentPass.h"

bool PresentPass::IsValid(){
    if(!RenderPass::IsValid())
        return false;
    if(!swapChainImageViews.has_value()){
        std::cerr << "swapChainImageViews is not initialized" << std::endl;
        return false;
    }
    if(!swapChainExtent.has_value()){
        std::cerr << "swapChainExtent is not initialized" << std::endl;
        return false;
    }
    if(!swapChainImageFormat.has_value()){
        std::cerr << "swapChainImageFormat is not initialized" << std::endl;
        return false;
    }
    if(!depthImageView.has_value()){
        std::cerr << "depthImageView is not initialized" << std::endl;
        return false;
    }
    return true;
}