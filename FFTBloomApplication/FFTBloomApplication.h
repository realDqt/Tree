//
// Created by 22473 on 2025-03-25.
//

#ifndef VKRENDERINGENGINE_FFTBLOOMAPPLICATION_H
#define VKRENDERINGENGINE_FFTBLOOMAPPLICATION_H
#include "FFTBloomUtils.h"
#include "../BaseApplication/BaseApplication.h"
#include "../Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "FBCubemapPass.h"
#include "FBSkyboxPass.h"
#include "NativeBloomPass.h"


class FFTBloomApplication : public BaseApplication{
    FBCubemapPass cubemapPasses[6];
    NativeBloomPass nativeBloomPass;
    FBSkyboxPass skyboxPass;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    // for cubemap passes
    VkImage depthImage2;
    VkDeviceMemory depthImageMemory2;
    VkImageView depthImageView2;

    VkImage hdrImage;
    VkDeviceMemory hdrImageMemory;
    VkImageView hdrImageView;
    VkSampler hdrSampler;
    VkExtent2D hdrImageExtent;

    VkImage kernelImage;
    VkDeviceMemory kernelImageMemory;
    VkImageView kernelImageView;
    VkSampler kernelSampler;

    // res
    VkImage bloomImage;
    VkDeviceMemory bloomImageMemory;
    VkImageView bloomImageView;
    VkSampler bloomSampler;

    // vb and ib for box
    std::vector<VertexBoxIBL> vertices;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkImage envCubemap;
    VkDeviceMemory envCubemapMemory;
    VkImageView envCubemapView;
    VkImageView envPerFaceViews[6];
    VkSampler envSampler;

    void prepareResources() override{
        // native bloom pass
        nativeBloomPass.device = device;
        nativeBloomPass.physicalDevice = physicalDevice;

        nativeBloomPass.hdrImageView = hdrImageView;
        nativeBloomPass.hdrSamper = hdrSampler;

        nativeBloomPass.kernelImageView = kernelImageView;
        nativeBloomPass.kernelSamper = kernelSampler;

        nativeBloomPass.bloomImageView = bloomImageView;

        nativeBloomPass.hdrTexSize = glm::uvec2(hdrImageExtent.width, hdrImageExtent.height);
        nativeBloomPass.R = R;
        nativeBloomPass.ratio = ratio;

        // cubemap passes
        glm::mat4 captureViews[6] =
                {
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),  // +X
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),  // -X
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),  // -Y
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f, 0.0f,  1.0f)),  // +Y
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),  // +Z
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))   // -Z
                };

        for(uint32_t faceIndex = 0; faceIndex < 6; ++faceIndex){
            cubemapPasses[faceIndex].device = device;
            cubemapPasses[faceIndex].physicalDevice = physicalDevice;

            cubemapPasses[faceIndex].hdrImageView = bloomImageView;
            cubemapPasses[faceIndex].hdrSampler = bloomSampler;

            cubemapPasses[faceIndex].colorImageView = envPerFaceViews[faceIndex];
            cubemapPasses[faceIndex].depthImageView = depthImageView2;

            cubemapPasses[faceIndex].vertexBuffer = vertexBuffer;
            cubemapPasses[faceIndex].indexBuffer = indexBuffer;
            cubemapPasses[faceIndex].indicesCount = indices.size();

            cubemapPasses[faceIndex].viewMat = captureViews[faceIndex];
        }

        // skybox pass
        skyboxPass.device = device;
        skyboxPass.physicalDevice = physicalDevice;


        skyboxPass.envCubemapView = envCubemapView;
        skyboxPass.envSampler = envSampler;

        skyboxPass.colorImageView = colorImageView;
        skyboxPass.depthImageView = depthImageView;

        skyboxPass.vertexBuffer = vertexBuffer;
        skyboxPass.indexBuffer = indexBuffer;
        skyboxPass.indicesCount = indices.size();

        skyboxPass.swapChainExtent = swapChainExtent;
        skyboxPass.swapChainImageFormat = swapChainImageFormat;
        skyboxPass.msaaSamples = msaaSamples;

        skyboxPass.swapChainImageViews.resize(swapChainImageViews.size());
        for(size_t i = 0; i < swapChainImageViews.size(); ++i)
            skyboxPass.swapChainImageViews[i] = swapChainImageViews[i];

    }

    void initVulkan() override{
        //camera.Position = lightPos;
        BaseApplication::initVulkan();
        createColorResources();
        createDepthResources();
        createEnvResources();
        createEnvSampler();


        createHdrImage(FB_HDR_TEXTURE_PATH, hdrImage, hdrImageMemory, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        createHdrSampler(hdrSampler);
        createHdrImageView(hdrImageView, hdrImage);

        createHdrImage(KERNEL_PATH, kernelImage, kernelImageMemory, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        createHdrSampler(kernelSampler);
        createHdrImageView(kernelImageView, kernelImage);

        createBloomImage();
        createHdrSampler(bloomSampler);
        createHdrImageView(bloomImageView, bloomImage);


        loadModel();
        createVertexBuffer();
        createIndexBuffer();


        prepareResources();
        nativeBloomPass.init();

        for(auto & cubemapPass : cubemapPasses){
            cubemapPass.init();
        }


        skyboxPass.init();

        executePrePass();
    }

    void executePrePass(){
        // native bloom pass
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        nativeBloomPass.updateUniformBuffer();
        nativeBloomPass.recordCommandBuffer(commandBuffer);

        endSingleTimeCommands(commandBuffer, true);

        transitionImageLayout(bloomImage, HDR_FORMAT, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);

        commandBuffer = beginSingleTimeCommands();
        // cubemap passes
        for(auto & cubemapPass : cubemapPasses){
            cubemapPass.recordCommandBuffer(commandBuffer);
        }

        generateCubemapMipmaps(commandBuffer, envCubemap, FB_CUBEMAP_RESOLUTION, FB_CUBEMAP_RESOLUTION, log2(FB_CUBEMAP_RESOLUTION) + 1, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        endSingleTimeCommands(commandBuffer);
    }

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        // skybox pass
        skyboxPass.recordCommandBuffer(commandBuffer, imageIndex);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(device);

        cleanupSwapChain();

        createSwapChain();
        createImageViews();
        createColorResources();
        createDepthResources();
    }

    void drawFrame() override{
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
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        //hdrVisualizationPass.currentFrame = currentFrame;
        skyboxPass.currentFrame = currentFrame;
    }

    void createColorResources() {
        VkFormat colorFormat = swapChainImageFormat;

        createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
        colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void createDepthResources() {
        VkFormat depthFormat = findDepthFormat();

        createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
        depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

        createImage(FB_CUBEMAP_RESOLUTION, FB_CUBEMAP_RESOLUTION, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage2, depthImageMemory2);
        depthImageView2 = createImageView(depthImage2, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

    }

    void createEnvResources() {
        VkFormat colorFormat = HDR_FORMAT;

        createCubemapImage(FB_CUBEMAP_RESOLUTION, FB_CUBEMAP_RESOLUTION, log2(FB_CUBEMAP_RESOLUTION) + 1, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, envCubemap, envCubemapMemory);
        envCubemapView = createCubemapImageView(envCubemap, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        for(uint32_t faceIndex = 0; faceIndex < 6; ++ faceIndex){
            envPerFaceViews[faceIndex] = createFthMthCubemapImageView(envCubemap, colorFormat,
                                                                      VK_IMAGE_ASPECT_COLOR_BIT, faceIndex);
        }
    }

    void createHdrImage(const std::string& imagePath, VkImage& image, VkDeviceMemory& imageMemory, VkImageUsageFlags usage) {
        // 使用 stbi_loadf 加载 HDR 图片
        int texWidth, texHeight, texChannels;
        float* pixels = stbi_loadf(imagePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        if(imagePath == FB_HDR_TEXTURE_PATH){
            hdrImageExtent = VkExtent2D(texWidth, texHeight);
        }
        VkDeviceSize imageSize = texWidth * texHeight * 4 * sizeof(float); // 每个通道占 4 字节（浮点数）
        // std::cout << "w: " << texWidth << " h: " << texHeight << std::endl;

        if (!pixels) {
            throw std::runtime_error("failed to load HDR texture image!");
        }

        // 创建 staging buffer
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(device, stagingBufferMemory);

        stbi_image_free(pixels); // 释放加载的 HDR 数据

        // 创建 HDR 图像
        createImage(texWidth, texHeight, 1, VK_SAMPLE_COUNT_1_BIT, HDR_FORMAT, VK_IMAGE_TILING_OPTIMAL, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, imageMemory);

        // 转换图像布局
        transitionImageLayout(image, HDR_FORMAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);
        copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        // 转换为 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);

        transitionImageLayout(image, HDR_FORMAT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);
    }

    void createBloomImage(){
        uint32_t texWidth, texHeight;
        texWidth = hdrImageExtent.width;
        texHeight = hdrImageExtent.height;
        createImage(texWidth, texHeight, 1, VK_SAMPLE_COUNT_1_BIT, HDR_FORMAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bloomImage, bloomImageMemory);
        transitionImageLayout(bloomImage, HDR_FORMAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL, 1);
    }



    void createHdrSampler(VkSampler& sampler) {
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

        if (vkCreateSampler(device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    void createEnvSampler() {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.anisotropyEnable = VK_FALSE;  // 是否启用各向异性过滤
        samplerInfo.maxAnisotropy = 1.0f;  // 各向异性过滤最大值
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &envSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    void createHdrImageView(VkImageView& imageView, VkImage& image) {
        imageView = createImageView(image, HDR_FORMAT, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void loadModel(){
        loadBoxModel();
    }




    void loadBoxModel() {
        float positions[] = {
                // 后面 (z = -0.5)
                -1.f, -1.f, -1.f, // v0
                1.f, -1.f, -1.f, // v1
                1.f,  1.f, -1.f, // v2
                -1.f,  1.f, -1.f, // v3

                // 前面 (z = 0.5)
                -1.f, -1.f,  1.f, // v4
                1.f, -1.f,  1.f, // v5
                1.f,  1.f,  1.f, // v6
                -1.f,  1.f,  1.f, // v7
        };

        float colors[] = {
                // 后面
                1.0f, 0.0f, 0.0f,  // v0
                1.0f, 0.0f, 0.0f,  // v1
                0.0f, 1.0f, 0.0f,  // v2
                1.0f, 0.0f, 0.0f,  // v3
                // 前面
                1.0f, 0.0f, 0.0f,  // v4
                1.0f, 0.0f, 0.0f,  // v5
                0.0f, 0.0f, 1.0f,  // v6
                1.0f, 0.0f, 0.0f   // v7
        };
        vertices.clear();
        for(int i = 0; i < 8; ++i){
            VertexBoxIBL vertex{};
            vertex.pos = glm::vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);
            vertices.push_back(vertex);
        }

        indices.clear();
        unsigned int ids[] = {
                // 后面 (z = -0.5)
                1, 0, 2,  // 第一个三角形
                3, 2, 0,  // 第二个三角形

                // 前面 (z = 0.5)
                4, 5, 6,  // 第一个三角形
                6, 7, 4,  // 第二个三角形

                // 左面 (x = -0.5)
                0, 4, 7,  // 第一个三角形
                7, 3, 0,  // 第二个三角形

                // 右面 (x = 0.5)
                1, 6, 5,  // 第一个三角形
                6, 1, 2,  // 第二个三角形

                // 底面 (y = -0.5)
                0, 1, 5,  // 第一个三角形
                5, 4, 0,  // 第二个三角形

                // 顶面 (y = 0.5)
                2, 3,6,  // 第一个三角形
                6, 3, 7   // 第二个三角形
        };
        for(unsigned int id : ids)
            indices.push_back(id);
    }

    void createVertexBuffer(){
        createVertexBoxBuffer();
    }

    void createIndexBuffer(){
        createIndexBoxBuffer();
    }

    void createVertexBoxBuffer() {
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



    void createIndexBoxBuffer() {
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

    void cleanupSwapChain() override{
        vkDestroyImageView(device, depthImageView, nullptr);
        vkDestroyImage(device, depthImage, nullptr);
        vkFreeMemory(device, depthImageMemory, nullptr);

        vkDestroyImageView(device, depthImageView2, nullptr);
        vkDestroyImage(device, depthImage2, nullptr);
        vkFreeMemory(device, depthImageMemory2, nullptr);


        vkDestroyImageView(device, colorImageView, nullptr);
        vkDestroyImage(device, colorImage, nullptr);
        vkFreeMemory(device, colorImageMemory, nullptr);


        /*
        for (auto framebuffer : hdrVisualizationPass.framebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        */

        for(auto & cubemapPass : cubemapPasses){
            for (auto framebuffer : cubemapPass.framebuffers) {
                vkDestroyFramebuffer(device, framebuffer, nullptr);
            }
        }

        for (auto framebuffer : skyboxPass.framebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    void cleanup() override{
        cleanupSwapChain();

        nativeBloomPass.cleanup();

        for(uint32_t faceIndex = 0; faceIndex < 6; ++faceIndex){
            cubemapPasses[faceIndex].cleanup();
            vkDestroyImageView(device, envPerFaceViews[faceIndex], nullptr);
        }

        skyboxPass.cleanup();



        vkDestroySampler(device, envSampler, nullptr);
        vkDestroyImageView(device, envCubemapView, nullptr);
        vkDestroyImage(device, envCubemap, nullptr);
        vkFreeMemory(device, envCubemapMemory, nullptr);


        vkDestroySampler(device, hdrSampler, nullptr);
        vkDestroyImageView(device, hdrImageView, nullptr);
        vkDestroyImage(device, hdrImage, nullptr);
        vkFreeMemory(device, hdrImageMemory, nullptr);

        vkDestroySampler(device, kernelSampler, nullptr);
        vkDestroyImageView(device, kernelImageView, nullptr);
        vkDestroyImage(device, kernelImage, nullptr);
        vkFreeMemory(device, kernelImageMemory, nullptr);

        vkDestroySampler(device, bloomSampler, nullptr);
        vkDestroyImageView(device, bloomImageView, nullptr);
        vkDestroyImage(device, bloomImage, nullptr);
        vkFreeMemory(device, bloomImageMemory, nullptr);



        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);


        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }

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
};
#endif //VKRENDERINGENGINE_FFTBLOOMAPPLICATION_H
