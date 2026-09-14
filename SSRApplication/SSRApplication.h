//
// Created by 22473 on 2025-03-03.
//

#ifndef VKRENDERINGENGINE_SSRAPPLICATION_H
#define VKRENDERINGENGINE_SSRAPPLICATION_H
#include "../BaseApplication/BaseApplication.h"
#include "../EngineCore/Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "SSRutils.h"
#include "SSRShadowmapPass.h"
#include "GBufferPass.h"
#include "GenerateHizPass.h"
#include "SSRPass.h"
#include "DenoisePass.h"
#include "FXAAPass.h"

class SSRApplication : public BaseApplication{
public:
    SSRShadowmapPass shadowmapPass;
    GBufferPass gBufferPasses[2];
    GenerateHizPass generateHizPass;
    SSRPass ssrPass;
    DenoisePass denoisePass;
    FXAAPass fxaaPass;

    // vb and ib for cube
    std::vector<VertexMarry> vertices;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    glm::mat4 cubeModel;

    // vb and ib for floor
    std::vector<VertexMarry> vertices2;
    std::vector<uint32_t> indices2;
    VkBuffer vertexBuffer2;
    VkDeviceMemory vertexBufferMemory2;
    VkBuffer indexBuffer2;
    VkDeviceMemory indexBufferMemory2;
    glm::mat4 floorModel;

    // vb and ib for quad
    std::vector<VertexQuad> vertices3;
    VkBuffer vertexBuffer3;
    VkDeviceMemory vertexBufferMemory3;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkImage depthImage2;
    VkDeviceMemory depthImageMemory2;
    VkImageView depthImageView2;

    VkImage shadowmap;
    VkDeviceMemory shadowmapMemory;
    VkImageView shadowmapView;
    VkSampler shadowmapSampler;


    uint32_t mipLevels;
    uint32_t hizMipLevels;
    bool hizInitialized = false;

    // resources for gBuffer
    VkImage gAlbedo;
    VkDeviceMemory gAlbedoMemory;
    VkImageView gAlbedoView;

    VkImage gWorldPosition;
    VkDeviceMemory gWorldPositionMemory;
    VkImageView gWorldPositionView;

    VkImage gWorldNormal;
    VkDeviceMemory gWorldNormalMemory;
    VkImageView gWorldNormalView;

    VkImage gDepth;
    VkDeviceMemory gDepthMemory;
    VkImageView gDepthView;
    VkImageView gDepthViewMip0;

    VkSampler gAlbedoSampler;
    VkSampler gWorldPositionSampler;
    VkSampler gWorldNormalSampler;
    VkSampler gDepthSampler;

    std::vector<VkImage> historyImages;
    std::vector<VkDeviceMemory> historyImageMemories;
    std::vector<VkImageView> historyImageViews;
    VkSampler historySampler;

    std::vector<VkImage> momentsImages;
    std::vector<VkDeviceMemory> momentsImageMemories;
    std::vector<VkImageView> momentsImageViews;

    std::vector<VkImage> directLightImages;
    std::vector<VkDeviceMemory> directLightImageMemories;
    std::vector<VkImageView> directLightImageViews;

    std::vector<VkImage> resolvedImages;
    std::vector<VkDeviceMemory> resolvedImageMemories;
    std::vector<VkImageView> resolvedImageViews;



    void prepareResources() override
    {

        // shadowmap pass
        shadowmapPass.device = device;
        shadowmapPass.physicalDevice = physicalDevice;

        shadowmapPass.colorImageView = shadowmapView;
        shadowmapPass.depthImageView = depthImageView2;

        shadowmapPass.vertexBufferMarry = vertexBuffer;
        shadowmapPass.indexBufferMarry = indexBuffer;
        shadowmapPass.indicesCountMarry = indices.size();

        shadowmapPass.vertexBufferFloor = vertexBuffer2;
        shadowmapPass.indexBufferFloor = indexBuffer2;
        shadowmapPass.indicesCountFloor = indices2.size();

        shadowmapPass.swapChainImagesCount = swapChainImageViews.size();
        shadowmapPass.cubeModel = cubeModel;
        shadowmapPass.floorModel = floorModel;
        shadowmapPass.currentFrame = currentFrame;

        // GBuffer Passes
        gBufferPasses[0].device = device;
        gBufferPasses[0].physicalDevice = physicalDevice;

        gBufferPasses[0].swapChainExtent = swapChainExtent;
        gBufferPasses[0].swapChainImageViewCount = swapChainImageViews.size();

        gBufferPasses[0].depthImageView = depthImageView;

        gBufferPasses[0].vertexBuffer = vertexBuffer;
        gBufferPasses[0].indexBuffer = indexBuffer;
        gBufferPasses[0].indicesCount = indices.size();

        gBufferPasses[0].textureImageView = textureImageView;
        gBufferPasses[0].textureSampler = textureSampler;

        gBufferPasses[0].gAlbedoView = gAlbedoView;
        gBufferPasses[0].gWorldPositionView = gWorldPositionView;
        gBufferPasses[0].gWorldNormalView = gWorldNormalView;
        gBufferPasses[0].gDepthView = gDepthViewMip0;

        gBufferPasses[0].model = cubeModel;
        gBufferPasses[0].isFloor = false;
        gBufferPasses[0].clearAttachments = true;
        gBufferPasses[0].currentFrame = currentFrame;


        gBufferPasses[1].device = device;
        gBufferPasses[1].physicalDevice = physicalDevice;

        gBufferPasses[1].swapChainExtent = swapChainExtent;
        gBufferPasses[1].swapChainImageViewCount = swapChainImageViews.size();

        gBufferPasses[1].depthImageView = depthImageView;

        gBufferPasses[1].vertexBuffer = vertexBuffer2;
        gBufferPasses[1].indexBuffer = indexBuffer2;
        gBufferPasses[1].indicesCount = indices2.size();

        gBufferPasses[1].textureImageView = textureImageView;
        gBufferPasses[1].textureSampler = textureSampler;

        gBufferPasses[1].gAlbedoView = gAlbedoView;
        gBufferPasses[1].gWorldPositionView = gWorldPositionView;
        gBufferPasses[1].gWorldNormalView = gWorldNormalView;
        gBufferPasses[1].gDepthView = gDepthViewMip0;

        gBufferPasses[1].model = floorModel;
        gBufferPasses[1].isFloor = true;
        gBufferPasses[1].clearAttachments = false;
        gBufferPasses[1].currentFrame = currentFrame;

        // generate hiz pass
        generateHizPass.device = device;
        generateHizPass.physicalDevice = physicalDevice;

        generateHizPass.hizImage = gDepth;
        generateHizPass.hizFormat = gDepthFormat;
        generateHizPass.hizWidth = swapChainExtent.width;
        generateHizPass.hizHeight = swapChainExtent.height;
        generateHizPass.mipLevels = hizMipLevels;

        // ssr pass
        ssrPass.device = device;
        ssrPass.physicalDevice = physicalDevice;

        ssrPass.swapChainExtent = swapChainExtent;

        ssrPass.depthImageView = depthImageView;

        ssrPass.vertexBuffer = vertexBuffer3;

        ssrPass.gAlbedoImageView = gAlbedoView;
        ssrPass.gAlbedoSampler = gAlbedoSampler;

        ssrPass.gWorldPositionImageView = gWorldPositionView;
        ssrPass.gWorldPositionSampler = gWorldPositionSampler;

        ssrPass.gWorldNormalImageView = gWorldNormalView;
        ssrPass.gWorldNormalSampler = gWorldNormalSampler;

        ssrPass.gDepthImageView = gDepthView;
        ssrPass.gDepthSampler = gDepthSampler;

        ssrPass.smImageView = shadowmapView;
        ssrPass.smSampler = shadowmapSampler;

        ssrPass.historyImageViews = historyImageViews;
        ssrPass.historySampler = historySampler;

        ssrPass.momentsImageViews = momentsImageViews;
        ssrPass.directLightImageViews = directLightImageViews;

        ssrPass.currentFrame = currentFrame; // test

        // denoise pass
        denoisePass.device = device;
        denoisePass.swapChainExtent = swapChainExtent;
        denoisePass.vertexBuffer = vertexBuffer3;

        denoisePass.indirectImageViews = historyImageViews;
        denoisePass.momentsImageViews = momentsImageViews;
        denoisePass.directLightImageViews = directLightImageViews;
        denoisePass.resolvedImageViews = resolvedImageViews;
        denoisePass.accumulationSampler = historySampler;

        denoisePass.gWorldPositionImageView = gWorldPositionView;
        denoisePass.gWorldPositionSampler = gWorldPositionSampler;
        denoisePass.gWorldNormalImageView = gWorldNormalView;
        denoisePass.gWorldNormalSampler = gWorldNormalSampler;
        denoisePass.gDepthImageView = gDepthView;
        denoisePass.gDepthSampler = gDepthSampler;
        denoisePass.gAlbedoImageView = gAlbedoView;
        denoisePass.gAlbedoSampler = gAlbedoSampler;

        denoisePass.maxAccumFrames = ssrPass.maxAccumFrames;
        denoisePass.currentFrame = currentFrame;

        fxaaPass.device = device;
        fxaaPass.swapChainExtent = swapChainExtent;
        fxaaPass.swapChainImageFormat = swapChainImageFormat;
        fxaaPass.swapChainImageViews = swapChainImageViews;
        fxaaPass.vertexBuffer = vertexBuffer3;
        fxaaPass.currentFrame = currentFrame;


    }
    void initVulkan() override{
        //camera.Position = lightPos;
        BaseApplication::initVulkan();
        createColorResources();
        createDepthResources();
        createGBufferResources();
        createHistoryResources();

        createTextureImage();
        createTextureImageView();
        createTextureSampler();

        createShadowmapSampler();
        createGSampler(gAlbedoSampler);
        createGSampler(gWorldPositionSampler, VK_FILTER_NEAREST);
        createGSampler(gWorldNormalSampler, VK_FILTER_NEAREST);
        createGSampler(gDepthSampler, VK_FILTER_NEAREST, hizMipLevels);


        loadModel();
        createVertexBuffer();
        createIndexBuffer();

        prepareResources();
        initPasses();
    }

    void cleanupSwapChain() override{
        for(auto& framebuffer : shadowmapPass.framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        shadowmapPass.framebuffers.clear();

        for(auto& framebuffer :gBufferPasses[0].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        gBufferPasses[0].framebuffers.clear();

        for(auto& framebuffer : gBufferPasses[1].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        gBufferPasses[1].framebuffers.clear();

        for(auto& framebuffer :ssrPass.framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        ssrPass.framebuffers.clear();

        for(auto& framebuffer : denoisePass.framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        denoisePass.framebuffers.clear();

        for(auto& framebuffer : fxaaPass.framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        fxaaPass.framebuffers.clear();

        vkDestroyImageView(device, colorImageView, nullptr);
        vkDestroyImage(device, colorImage, nullptr);
        vkFreeMemory(device, colorImageMemory, nullptr);

        vkDestroyImageView(device, depthImageView, nullptr);
        vkDestroyImage(device, depthImage, nullptr);
        vkFreeMemory(device, depthImageMemory, nullptr);

        vkDestroyImageView(device, depthImageView2, nullptr);
        vkDestroyImage(device, depthImage2, nullptr);
        vkFreeMemory(device, depthImageMemory2, nullptr);

        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }
        swapChainImageViews.clear();
        swapChainImages.clear();

        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    void cleanup() override{
        destroySyncResources();
        cleanupSwapChain();
        cleanupPasses();
        destroyHistoryResources();
        destroyGBufferResources();
        destroyShadowmapResources();

        vkDestroyImageView(device, textureImageView, nullptr);
        vkDestroySampler(device, textureSampler, nullptr);
        vkDestroyImage(device, textureImage, nullptr);
        vkFreeMemory(device, textureImageMemory, nullptr);

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);

        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);

        vkDestroyBuffer(device, vertexBuffer2, nullptr);
        vkFreeMemory(device, vertexBufferMemory2, nullptr);

        vkDestroyBuffer(device, indexBuffer2, nullptr);
        vkFreeMemory(device, indexBufferMemory2, nullptr);

        vkDestroyBuffer(device, vertexBuffer3, nullptr);
        vkFreeMemory(device, vertexBufferMemory3, nullptr);

        vkDestroyCommandPool(device, commandPool, nullptr);

        vkDestroyDevice(device, nullptr);

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
    }

    void initPasses() {
        shadowmapPass.init();
        gBufferPasses[0].init();
        gBufferPasses[1].init();
        generateHizPass.init();
        ssrPass.init();
        denoisePass.init();
        fxaaPass.inputImageViews = resolvedImageViews;
        fxaaPass.inputSampler = historySampler;
        fxaaPass.init();
    }

    void cleanupPasses() {
        shadowmapPass.cleanup();
        gBufferPasses[0].cleanup();
        gBufferPasses[1].cleanup();
        generateHizPass.cleanup();
        fxaaPass.cleanup();
        denoisePass.cleanup();
        ssrPass.cleanup();
    }

    void destroyHistoryResources() {
        vkDestroySampler(device, historySampler, nullptr);
        destroyRingImages(historyImages, historyImageMemories, historyImageViews);
        destroyRingImages(momentsImages, momentsImageMemories, momentsImageViews);
        destroyRingImages(directLightImages, directLightImageMemories, directLightImageViews);
        destroyRingImages(resolvedImages, resolvedImageMemories, resolvedImageViews);
    }

    void destroyGBufferResources() {
        vkDestroyImageView(device, gAlbedoView, nullptr);
        vkDestroyImage(device, gAlbedo, nullptr);
        vkFreeMemory(device, gAlbedoMemory, nullptr);
        vkDestroySampler(device, gAlbedoSampler, nullptr);

        vkDestroyImageView(device, gWorldPositionView, nullptr);
        vkDestroyImage(device, gWorldPosition, nullptr);
        vkFreeMemory(device, gWorldPositionMemory, nullptr);
        vkDestroySampler(device, gWorldPositionSampler, nullptr);

        vkDestroyImageView(device, gWorldNormalView, nullptr);
        vkDestroyImage(device, gWorldNormal, nullptr);
        vkFreeMemory(device, gWorldNormalMemory, nullptr);
        vkDestroySampler(device, gWorldNormalSampler, nullptr);

        vkDestroyImageView(device, gDepthView, nullptr);
        vkDestroyImageView(device, gDepthViewMip0, nullptr);
        vkDestroyImage(device, gDepth, nullptr);
        vkFreeMemory(device, gDepthMemory, nullptr);
        vkDestroySampler(device, gDepthSampler, nullptr);
    }

    void destroyShadowmapResources() {
        vkDestroyImageView(device, shadowmapView, nullptr);
        vkDestroyImage(device, shadowmap, nullptr);
        vkFreeMemory(device, shadowmapMemory, nullptr);
        vkDestroySampler(device, shadowmapSampler, nullptr);
    }

    void destroySyncResources() {
        for (auto semaphore : renderFinishedSemaphores) {
            vkDestroySemaphore(device, semaphore, nullptr);
        }
        for (auto semaphore : imageAvailableSemaphores) {
            vkDestroySemaphore(device, semaphore, nullptr);
        }
        for (auto fence : inFlightFences) {
            vkDestroyFence(device, fence, nullptr);
        }
        if (!commandBuffers.empty()) {
            vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        }
        renderFinishedSemaphores.clear();
        imageAvailableSemaphores.clear();
        inFlightFences.clear();
        commandBuffers.clear();
    }

    void createVertexBuffer(){
        createCubeVertexBuffer();
        createFloorVertexBuffer();
        createQuadVertexBuffer();
    }

    void createIndexBuffer(){
        createCubeIndexBuffer();
        createFloorIndexBuffer();
    }

    void createCubeVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void createFloorVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(vertices2[0]) * vertices2.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices2.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer2, vertexBufferMemory2);

        copyBuffer(stagingBuffer, vertexBuffer2, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void createQuadVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(vertices3[0]) * vertices3.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices3.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer3, vertexBufferMemory3);

        copyBuffer(stagingBuffer, vertexBuffer3, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void createCubeIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void createFloorIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(indices2[0]) * indices2.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices2.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer2, indexBufferMemory2);

        copyBuffer(stagingBuffer, indexBuffer2, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void createShadowmapSampler() {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_NEAREST;
        samplerInfo.minFilter = VK_FILTER_NEAREST;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &shadowmapSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shadow map sampler!");
        }
    }

    // The G-Buffer is read through plain sampler2D, so compare sampling must stay off.
    // Attributes that must not be interpolated across depth discontinuities (world position,
    // world normal, Hi-Z depth) pass VK_FILTER_NEAREST; only albedo is filtered linearly.
    void createGSampler(VkSampler& gSampler, VkFilter filter = VK_FILTER_LINEAR, uint32_t samplerMipLevels = 1) {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = filter;
        samplerInfo.minFilter = filter;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;  // 边界模式
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;  // 边界颜色为白色
        samplerInfo.anisotropyEnable = VK_FALSE;  // 关闭各向异性过滤
        samplerInfo.maxAnisotropy = 1.0f;  // 设置为 1.0（无效，但需要初始化）
        samplerInfo.unnormalizedCoordinates = VK_FALSE;  // 使用标准化坐标
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = static_cast<float>(samplerMipLevels - 1);
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &gSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shadow map sampler!");
        }
    }



    void recreateSwapChain() {
        framebufferResized = false;

        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(device);

        destroySyncResources();
        cleanupSwapChain();
        cleanupPasses();
        destroyHistoryResources();
        destroyGBufferResources();
        destroyShadowmapResources();

        currentFrame = 0;
        createSwapChain();
        createImageViews();
        createCommandBuffers();
        createSyncObjects();
        createColorResources();
        createDepthResources();
        createGBufferResources();
        createHistoryResources();
        createShadowmapSampler();
        createGSampler(gAlbedoSampler);
        createGSampler(gWorldPositionSampler, VK_FILTER_NEAREST);
        createGSampler(gWorldNormalSampler, VK_FILTER_NEAREST);
        createGSampler(gDepthSampler, VK_FILTER_NEAREST, hizMipLevels);
        prepareResources();
        initPasses();

        hizInitialized = false;
        ssrPass.historyValid = false;
        ssrPass.temporalFrameIndex = 0;
    }

    void loadModel(){
        loadCube();
        loadFloor();
        loadQuadModel();
    }


    void loadQuadModel(){
        float positions[] = {
                -1.f, 1.f, 0.f,
                -1.f, -1.f, 0.f,
                1.f, 1.f, 0.f,
                1.f, -1.f, 0.f
        };

        float texCoords[] = {
                0.f, 1.f,
                0.f, 0.f,
                1.f, 1.f,
                1.f, 0.f
        };

        vertices3.clear();
        for(uint32_t i = 0; i < 4; ++i){
            VertexQuad vertexQuad{};
            vertexQuad.pos = glm::vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);
            vertexQuad.texCoord = glm::vec2(texCoords[i * 2], texCoords[i * 2 + 1]);

            vertices3.push_back(vertexQuad);
        }
    }

    void loadCube(){
        // 创建 Assimp 导入器
        Assimp::Importer importer;

        // 加载 glTF 文件
        const aiScene* scene = importer.ReadFile(CUBE_PATH, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            throw std::runtime_error("Failed to load GLTF file: " + std::string(importer.GetErrorString()));
        }

        // 遍历场景中的节点
        std::function<void(aiNode*, const aiMatrix4x4&)> processNode = [&](aiNode* node, const aiMatrix4x4& parentTransform) {
            aiMatrix4x4 currentTransform = parentTransform * node->mTransformation;

            // 检查节点名称是否为 "Cube"
            if (std::string(node->mName.C_Str()) == "Cube") {
                // 提取变换矩阵
                cubeModel = aiMat2glmMat(currentTransform);

                // 遍历该节点的所有网格
                for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

                    // 提取顶点数据
                    for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                        VertexMarry vertex{};

                        // 位置
                        vertex.pos = aiVec2glmVec(mesh->mVertices[j]);

                        // 法向量
                        if (mesh->HasNormals()) {
                            vertex.normal = aiVec2glmVec(mesh->mNormals[j]);
                        }

                        // 纹理坐标
                        if (mesh->HasTextureCoords(0)) { // 检查是否有纹理坐标
                            vertex.texCoord = glm::vec2(
                                    mesh->mTextureCoords[0][j].x,
                                    mesh->mTextureCoords[0][j].y
                            );
                        } else {
                            vertex.texCoord = glm::vec2(0.0f, 0.0f); // 默认值
                        }

                        vertices.push_back(vertex);
                    }

                    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
                        const aiFace& face = mesh->mFaces[i];
                        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                            indices.push_back(face.mIndices[j]);
                        }
                    }
                }

            }

            // 递归处理子节点
            for (unsigned int i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], currentTransform);
            }
        };
        // 从根节点开始处理
        processNode(scene->mRootNode, aiMatrix4x4());
    }

    void loadFloor(){
        // 创建 Assimp 导入器
        Assimp::Importer importer;

        // 加载 glTF 文件
        const aiScene* scene = importer.ReadFile(CUBE_PATH, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            throw std::runtime_error("Failed to load GLTF file: " + std::string(importer.GetErrorString()));
        }

        // 遍历场景中的节点
        std::function<void(aiNode*, const aiMatrix4x4&)> processNode = [&](aiNode* node, const aiMatrix4x4& parentTransform) {
            aiMatrix4x4 currentTransform = parentTransform * node->mTransformation;

            // 检查节点名称是否为 "Cube"
            if (std::string(node->mName.C_Str()) == "Plane") {
                // 提取变换矩阵
                floorModel = aiMat2glmMat(currentTransform);

                // 遍历该节点的所有网格
                for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

                    // 提取顶点数据
                    for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                        VertexMarry vertex{};

                        // 位置
                        vertex.pos = aiVec2glmVec(mesh->mVertices[j]);

                        // 法向量
                        if (mesh->HasNormals()) {
                            vertex.normal = aiVec2glmVec(mesh->mNormals[j]);
                        }

                        // 纹理坐标
                        if (mesh->HasTextureCoords(0)) { // 检查是否有纹理坐标
                            vertex.texCoord = glm::vec2(
                                    mesh->mTextureCoords[0][j].x,
                                    mesh->mTextureCoords[0][j].y
                            );
                        } else {
                            vertex.texCoord = glm::vec2(0.0f, 0.0f); // 默认值
                        }

                        vertices2.push_back(vertex);
                    }

                    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
                        const aiFace& face = mesh->mFaces[i];
                        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                            indices2.push_back(face.mIndices[j]);
                        }
                    }
                }

            }

            // 递归处理子节点
            for (unsigned int i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], currentTransform);
            }
        };

        // 从根节点开始处理
        processNode(scene->mRootNode, aiMatrix4x4());
    }

    void createColorResources() {
        VkFormat colorFormat = swapChainImageFormat;

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
        colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        colorFormat = shadowmapFormat;
        createImage(SM_RESOLUTION, SM_RESOLUTION, 1, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,  shadowmap, shadowmapMemory);
        shadowmapView = createImageView(shadowmap, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void createGBufferResources(){
        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, gAlbedoFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, gAlbedo, gAlbedoMemory);
        gAlbedoView = createImageView(gAlbedo, gAlbedoFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, gWorldPositionFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, gWorldPosition, gWorldPositionMemory);
        gWorldPositionView = createImageView(gWorldPosition, gWorldPositionFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, gWorldNormalFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, gWorldNormal, gWorldNormalMemory);
        gWorldNormalView = createImageView(gWorldNormal, gWorldNormalFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);


        hizMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(swapChainExtent.width, swapChainExtent.height)))) + 1;
        createImage(swapChainExtent.width, swapChainExtent.height, hizMipLevels, VK_SAMPLE_COUNT_1_BIT, gDepthFormat, VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, gDepth, gDepthMemory);
        gDepthView = createImageView(gDepth, gDepthFormat, VK_IMAGE_ASPECT_COLOR_BIT, hizMipLevels);
        gDepthViewMip0 = createImageView(gDepth, gDepthFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    // One full screen target per frame in flight, left in SHADER_READ_ONLY_OPTIMAL so the
    // render passes that produce them can declare it as both initial and final layout.
    void createRingImages(VkFormat format,
                          std::vector<VkImage>& images,
                          std::vector<VkDeviceMemory>& memories,
                          std::vector<VkImageView>& views) {
        images.resize(MAX_FRAMES_IN_FLIGHT);
        memories.resize(MAX_FRAMES_IN_FLIGHT);
        views.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < images.size(); i++) {
            createImage(
                    swapChainExtent.width,
                    swapChainExtent.height,
                    1,
                    VK_SAMPLE_COUNT_1_BIT,
                    format,
                    VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    images[i],
                    memories[i]
            );
            views[i] = createImageView(images[i], format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
            transitionImageLayout(images[i], format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);
            transitionImageLayout(images[i], format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);
        }
    }

    void destroyRingImages(std::vector<VkImage>& images,
                           std::vector<VkDeviceMemory>& memories,
                           std::vector<VkImageView>& views) {
        for (size_t i = 0; i < images.size(); i++) {
            vkDestroyImageView(device, views[i], nullptr);
            vkDestroyImage(device, images[i], nullptr);
            vkFreeMemory(device, memories[i], nullptr);
        }
        views.clear();
        images.clear();
        memories.clear();
    }

    void createHistoryResources() {
        createRingImages(historyFormat, historyImages, historyImageMemories, historyImageViews);
        createRingImages(momentsFormat, momentsImages, momentsImageMemories, momentsImageViews);
        createRingImages(directLightFormat, directLightImages, directLightImageMemories, directLightImageViews);
        createRingImages(resolvedFormat, resolvedImages, resolvedImageMemories, resolvedImageViews);

        // SSR reads the history at exact texel centres, but FXAA samples it at fractional offsets.
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &historySampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create history sampler!");
        }
    }

    void createDepthResources() {
        VkFormat depthFormat = findDepthFormat();

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
        depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

        createImage(SM_RESOLUTION, SM_RESOLUTION, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage2, depthImageMemory2);
        depthImageView2 = createImageView(depthImage2, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void createTextureImage() {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(CUBE_TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;
        mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

        if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(device, stagingBufferMemory);

        stbi_image_free(pixels);

        createImage(texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

        transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
        copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        //transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);

        generateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
    }

    void createTextureSampler() {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    void createTextureImageView() {
        textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
    }


    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        shadowmapPass.recordCommandBuffer(commandBuffer, imageIndex);
        gBufferPasses[0].recordCommandBuffer(commandBuffer, imageIndex);
        gBufferPasses[1].recordCommandBuffer(commandBuffer, imageIndex);


        if (hizMipLevels > 1) {
            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.image = gDepth;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            barrier.srcAccessMask = hizInitialized ? VK_ACCESS_SHADER_READ_BIT : 0;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;

            barrier.oldLayout = hizInitialized ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 1;
            barrier.subresourceRange.levelCount = hizMipLevels  - 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            vkCmdPipelineBarrier(
                    commandBuffer,
                    hizInitialized ? VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT : VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &barrier
            );
        }

        generateHizPass.recordCommandBuffer(commandBuffer);
        hizInitialized = true;

        //blinnPhongPasses[0].recordCommandBuffer(commandBuffer, imageIndex);
        //blinnPhongPasses[1].recordCommandBuffer(commandBuffer, imageIndex);
        ssrPass.recordCommandBuffer(commandBuffer);
        denoisePass.recordCommandBuffer(commandBuffer);
        fxaaPass.recordCommandBuffer(commandBuffer, imageIndex);


        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

    }



    void drawFrame() override{
        //std::cout << "SSRApplication: call drawFrame" << std::endl;
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }


        vkResetFences(device, 1, &inFlightFences[currentFrame]);

        vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
        recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            framebufferResized = false;
            recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        shadowmapPass.currentFrame = currentFrame;
        gBufferPasses[0].currentFrame = currentFrame;
        gBufferPasses[1].currentFrame = currentFrame;
        ssrPass.currentFrame = currentFrame;
        denoisePass.currentFrame = currentFrame;
        fxaaPass.currentFrame = currentFrame;
    }
};

#endif //VKRENDERINGENGINE_SSRAPPLICATION_H
