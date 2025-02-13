//
// Created by 22473 on 2025-01-20.
//

#ifndef VKRENDERINGENGINE_IBLAPPLICATION_H
#define VKRENDERINGENGINE_IBLAPPLICATION_H
#include "../BaseApplication/BaseApplication.h"
#include "../Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "HDRVisualizationPass.h"
#include "CubemapPass.h"
#include "SkyboxPass.h"
#include "SpherePass.h"

class IBLApplication : public BaseApplication{
public:
    //HDRVisualizationPass hdrVisualizationPass;
    CubemapPass cubemapPasses[6];
    SkyboxPass skyboxPass;
    SpherePass spherePasses[nrRows * nrColumns];

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    // for situation without msaa
    VkImage depthImage2;
    VkDeviceMemory depthImageMemory2;
    VkImageView depthImageView2;

    VkImage hdrImage;
    VkDeviceMemory hdrImageMemory;
    VkImageView hdrImageView;
    VkSampler hdrSampler;

    // vb and ib for box
    std::vector<VertexBoxIBL> vertices;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    // vb and ib for sphere
    std::vector<VertexSphere> vertices2;
    std::vector<uint32_t> indices2;
    VkBuffer vertexBuffer2;
    VkDeviceMemory vertexBufferMemory2;
    VkBuffer indexBuffer2;
    VkDeviceMemory indexBufferMemory2;

    VkImage envCubemap;
    VkDeviceMemory envCubemapMemory;
    VkImageView envCubemapView;
    VkImageView envPerFaceViews[6];
    VkSampler envSampler;



    void prepareResources() override
    {
        /*
        // hdr visualization pass
        hdrVisualizationPass.device = device;
        hdrVisualizationPass.physicalDevice = physicalDevice;

        hdrVisualizationPass.hdrImageView = hdrImageView;
        hdrVisualizationPass.hdrSampler = hdrSampler;

        hdrVisualizationPass.colorImageView = colorImageView;
        hdrVisualizationPass.depthImageView = depthImageView;

        hdrVisualizationPass.vertexBuffer = vertexBuffer;
        hdrVisualizationPass.indexBuffer = indexBuffer;
        hdrVisualizationPass.indicesCount = indices.size();

        hdrVisualizationPass.swapChainExtent = swapChainExtent;
        hdrVisualizationPass.swapChainImageFormat = swapChainImageFormat;
        hdrVisualizationPass.msaaSamples = msaaSamples;

        hdrVisualizationPass.swapChainImageViews.resize(swapChainImageViews.size());
        hdrVisualizationPass.viewMatTest = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));

        for(size_t i = 0; i < swapChainImageViews.size(); ++i)
            hdrVisualizationPass.swapChainImageViews[i] = swapChainImageViews[i];
        */

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

            cubemapPasses[faceIndex].hdrImageView = hdrImageView;
            cubemapPasses[faceIndex].hdrSampler = hdrSampler;

            cubemapPasses[faceIndex].colorImageView = envPerFaceViews[faceIndex];
            cubemapPasses[faceIndex].depthImageView = depthImageView2;

            cubemapPasses[faceIndex].vertexBuffer = vertexBuffer;
            cubemapPasses[faceIndex].indexBuffer = indexBuffer;
            cubemapPasses[faceIndex].indicesCount = indices.size();

            cubemapPasses[faceIndex].swapChainImageViewsCount = swapChainImageViews.size();
            cubemapPasses[faceIndex].viewMat = captureViews[faceIndex];
        }

        // sphere passes
        auto model = glm::mat4(1.0f);
        for (int row = 0; row < nrRows; ++row)
        {
            SpherePass::ExternalInfo externalInfo{};
            externalInfo.metallic = (float)row / (float)nrRows;
            externalInfo.albedo = glm::vec3(0.5f, 0.0f, 0.0f);
            externalInfo.ao = 1.0f;
            for (int col = 0; col < nrColumns; ++col)
            {
                // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                externalInfo.roughness = glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(
                        (float)(col - (nrColumns / 2)) * spacing,
                        (float)(row - (nrRows / 2)) * spacing,
                        -2.0f
                ));
                externalInfo.model = model;
                externalInfo.normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
                int idx = row * nrColumns + col;
                spherePasses[idx].device = device;
                spherePasses[idx].physicalDevice = physicalDevice;

                spherePasses[idx].colorImageView = colorImageView;
                spherePasses[idx].depthImageView = depthImageView;

                spherePasses[idx].vertexBuffer = vertexBuffer2;
                spherePasses[idx].indexBuffer = indexBuffer2;
                spherePasses[idx].indicesCount = indices2.size();

                spherePasses[idx].swapChainExtent = swapChainExtent;
                spherePasses[idx].swapChainImageFormat = swapChainImageFormat;
                spherePasses[idx].msaaSamples = msaaSamples;

                spherePasses[idx].swapChainImageViews.resize(swapChainImageViews.size());

                for(size_t i = 0; i < swapChainImageViews.size(); ++i)
                    spherePasses[idx].swapChainImageViews[i] = swapChainImageViews[i];
                spherePasses[idx].externalInfo = externalInfo;
            }
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

        createHdrImage();
        createHdrSampler();
        createHdrImageView();

        loadModel();
        createVertexBuffer();
        createIndexBuffer();


        prepareResources();
        //hdrVisualizationPass.init();
        for(auto & cubemapPass : cubemapPasses){
            cubemapPass.init();
        }

        for(auto& spherePass : spherePasses){
            spherePass.init();
        }

        skyboxPass.init();

        executePrePass();
    }

    void executePrePass(){
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();
        // cubemap passes
        for(auto & cubemapPass : cubemapPasses){
            cubemapPass.recordCommandBuffer(commandBuffer);
        }

        //copyImage2DtoCubemap(commandBuffer, colorImages, envCubemap, CUBEMAP_RESOLUTION);

        endSingleTimeCommands(commandBuffer);

    }

    void copyImage2DtoCubemap(VkCommandBuffer cmdBuf, std::vector<VkImage>& images, VkImage& cubemap, uint32_t dim){
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.layerCount = 6;

        // Change image layout for all cubemap faces to transfer destination
        vks::tools::setImageLayout(
                cmdBuf,
                cubemap,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                subresourceRange);

        for (uint32_t f = 0; f < 6; f++) {
            vks::tools::setImageLayout(
                    cmdBuf,
                    images[f],
                    VK_IMAGE_ASPECT_COLOR_BIT,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

            // Copy region for transfer from framebuffer to cube face
            VkImageCopy copyRegion = {};

            copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.srcSubresource.baseArrayLayer = 0;
            copyRegion.srcSubresource.mipLevel = 0;
            copyRegion.srcSubresource.layerCount = 1;
            copyRegion.srcOffset = { 0, 0, 0 };

            copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.dstSubresource.baseArrayLayer = f;
            copyRegion.dstSubresource.mipLevel = 0;
            copyRegion.dstSubresource.layerCount = 1;
            copyRegion.dstOffset = { 0, 0, 0 };

            copyRegion.extent.width = static_cast<uint32_t>(dim);
            copyRegion.extent.height = static_cast<uint32_t>(dim);
            copyRegion.extent.depth = 1;

            vkCmdCopyImage(
                    cmdBuf,
                    images[f],
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    cubemap,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1,
                    &copyRegion);
        }

        vks::tools::setImageLayout(
                cmdBuf,
                cubemap,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                subresourceRange);
    }


    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }


        // Light visualization pass
        //hdrVisualizationPass.recordCommandBuffer(commandBuffer, imageIndex);

        // sphere passes
        for(auto& spherePass : spherePasses){
            spherePass.recordCommandBuffer(commandBuffer, imageIndex);
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
        for(auto& spherePass : spherePasses)
            spherePass.currentFrame = currentFrame;
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

        createImage(CUBEMAP_RESOLUTION, CUBEMAP_RESOLUTION, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage2, depthImageMemory2);
        depthImageView2 = createImageView(depthImage2, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void createEnvResources() {
        VkFormat colorFormat = VK_FORMAT_R8G8B8A8_SRGB;

        createCubemapImage(CUBEMAP_RESOLUTION, CUBEMAP_RESOLUTION, 1, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, envCubemap, envCubemapMemory);
        envCubemapView = createCubemapImageView(envCubemap, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        for(uint32_t faceIndex = 0; faceIndex < 6; ++ faceIndex){
            envPerFaceViews[faceIndex] = createKthCubemapImageView(envCubemap, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, faceIndex);
        }
    }

    void createHdrImage() {
        //stbi_set_flip_vertically_on_load(true);
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(HDR_TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

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

        createImage(texWidth, texHeight, 1, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, hdrImage, hdrImageMemory);

        transitionImageLayout(hdrImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);
        copyBufferToImage(stagingBuffer, hdrImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        //transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);

        generateMipmaps(hdrImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, 1);
    }


    void createHdrSampler() {
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

        if (vkCreateSampler(device, &samplerInfo, nullptr, &hdrSampler) != VK_SUCCESS) {
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

    void createHdrImageView() {
        hdrImageView = createImageView(hdrImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void loadModel(){
        loadBoxModel();
        loadSphereModel();
    }

    void loadSphereModel(){
        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                VertexSphere vertexSphere{};
                vertexSphere.pos = glm::vec3(xPos, yPos, zPos);
                vertexSphere.normal = glm::vec3(xPos, yPos, zPos);
                vertexSphere.texCoord = glm::vec2(xSegment, ySegment);
                vertices2.push_back(vertexSphere);
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices2.push_back(y * (X_SEGMENTS + 1) + x);
                    indices2.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices2.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices2.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
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
        createVertexSphereBuffer();
    }

    void createIndexBuffer(){
        createIndexBoxBuffer();
        createIndexSphereBuffer();
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

    void createVertexSphereBuffer() {
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

    void createIndexSphereBuffer() {
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
        for(auto & spherePass : spherePasses){
            for (auto framebuffer : spherePass.framebuffers) {
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

        //hdrVisualizationPass.cleanup();
        for(uint32_t faceIndex = 0; faceIndex < 6; ++faceIndex){
            cubemapPasses[faceIndex].cleanup();
            vkDestroyImageView(device, envPerFaceViews[faceIndex], nullptr);
        }
        skyboxPass.cleanup();
        for(auto& spherePass: spherePasses){
            spherePass.cleanup();
        }


        vkDestroyImageView(device, envCubemapView, nullptr);
        vkDestroyImage(device, envCubemap, nullptr);
        vkFreeMemory(device, envCubemapMemory, nullptr);

        vkDestroySampler(device, hdrSampler, nullptr);
        vkDestroyImageView(device, hdrImageView, nullptr);

        vkDestroyImage(device, hdrImage, nullptr);
        vkFreeMemory(device, hdrImageMemory, nullptr);

        vkDestroySampler(device, envSampler, nullptr);



        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);

        vkDestroyBuffer(device, indexBuffer2, nullptr);
        vkFreeMemory(device, indexBufferMemory2, nullptr);

        vkDestroyBuffer(device, vertexBuffer2, nullptr);
        vkFreeMemory(device, vertexBufferMemory2, nullptr);


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
#endif //VKRENDERINGENGINE_IBLAPPLICATION_H
