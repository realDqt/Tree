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
#include "IrradianceMapPass.h"
#include "PrefilterMapPass.h"
#include "BrdfLUTPass.h"

class IBLApplication : public BaseApplication{
public:
    //HDRVisualizationPass hdrVisualizationPass;
    CubemapPass cubemapPasses[6];
    IrradianceMapPass irradianceMapPasses[6];
    PrefilterMapPass prefilterMapPasses[6 * MIPMAP_COUNT];
    BrdfLUTPass brdfLUTPass;



    SkyboxPass skyboxPass;
    SpherePass spherePasses[NR_ROWS * NR_COLUMNS];

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

    // for irradiance map passes
    VkImage depthImage3;
    VkDeviceMemory depthImageMemory3;
    VkImageView depthImageView3;

    // for prefilter map passes
    VkImage depthImages[6 * MIPMAP_COUNT];
    VkDeviceMemory depthImageMemories[6 * MIPMAP_COUNT];
    VkImageView depthImageViews[6 * MIPMAP_COUNT];

    // for brdfLUT pass
    VkImage depthImage4;
    VkDeviceMemory depthImageMemory4;
    VkImageView depthImageView4;

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

    // vb and ib for quad
    std::vector<VertexQuad> vertices3;
    VkBuffer vertexBuffer3;
    VkDeviceMemory vertexBufferMemory3;

    VkImage envCubemap;
    VkDeviceMemory envCubemapMemory;
    VkImageView envCubemapView;
    VkImageView envPerFaceViews[6];
    VkSampler envSampler;

    VkImage irradianceMap;
    VkDeviceMemory irradianceMapMemory;
    VkImageView irradianceMapView;
    VkImageView irradiancePerFaceViews[6];
    VkSampler irradianceSampler;

    VkImage prefilterMap;
    VkDeviceMemory prefilterMapMemory;
    VkImageView prefilterMapView;
    VkImageView prefilterPerFacePerMipViews[6 * MIPMAP_COUNT];
    VkSampler prefilterMapSampler;

    VkImage brdfLUT;
    VkDeviceMemory brdfLUTMemory;
    VkImageView brdfLUTView;
    VkSampler brdfLUTSampler;



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

            cubemapPasses[faceIndex].viewMat = captureViews[faceIndex];
        }

        // irradiance passes
        for(uint32_t faceIndex = 0; faceIndex < 6; ++faceIndex){
            irradianceMapPasses[faceIndex].device = device;
            irradianceMapPasses[faceIndex].physicalDevice = physicalDevice;

            irradianceMapPasses[faceIndex].envImageView = envCubemapView;
            irradianceMapPasses[faceIndex].envSampler = envSampler;

            irradianceMapPasses[faceIndex].colorImageView = irradiancePerFaceViews[faceIndex];
            irradianceMapPasses[faceIndex].depthImageView = depthImageView3;

            irradianceMapPasses[faceIndex].vertexBuffer = vertexBuffer;
            irradianceMapPasses[faceIndex].indexBuffer = indexBuffer;
            irradianceMapPasses[faceIndex].indicesCount = indices.size();

            irradianceMapPasses[faceIndex].viewMat = captureViews[faceIndex];
        }

        glm::mat4 prefilterCaptureViews[6] =
                {
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),  // +X
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),  // -X
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f, 0.0f,  1.0f)),  // +Y
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),  // -Y
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),  // +Z
                        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))   // -Z

                };
        // prefilter map passes
        uint32_t resolution = PREFILTER_MAP_RESOLUTION;
        for(uint32_t mipLevel = 0; mipLevel < MIPMAP_COUNT; ++mipLevel){
            for(uint32_t faceIndex = 0; faceIndex < 6; ++faceIndex){
                uint32_t idx = mipLevel * 6 + faceIndex;
                prefilterMapPasses[idx].device = device;
                prefilterMapPasses[idx].physicalDevice = physicalDevice;

                prefilterMapPasses[idx].envImageView = envCubemapView;
                prefilterMapPasses[idx].envSampler = envSampler;

                prefilterMapPasses[idx].colorImageView = prefilterPerFacePerMipViews[idx];
                prefilterMapPasses[idx].depthImageView = depthImageViews[idx];

                prefilterMapPasses[idx].vertexBuffer = vertexBuffer;
                prefilterMapPasses[idx].indexBuffer = indexBuffer;
                prefilterMapPasses[idx].indicesCount = indices.size();

                prefilterMapPasses[idx].viewMat = prefilterCaptureViews[faceIndex];
                prefilterMapPasses[idx].colorImageExtent = VkExtent2D(resolution, resolution);
                prefilterMapPasses[idx].roughness = (float)mipLevel / (float)(MIPMAP_COUNT - 1);
            }
            resolution >>= 1;
        }

        // brdfLUT pass
        brdfLUTPass.device = device;
        brdfLUTPass.physicalDevice = physicalDevice;

        brdfLUTPass.colorImageView = brdfLUTView;
        brdfLUTPass.depthImageView = depthImageView4;

        brdfLUTPass.vertexBuffer = vertexBuffer3;


        // sphere passes
        auto model = glm::mat4(1.0f);
        for (int row = 0; row < NR_ROWS; ++row)
        {
            SpherePass::ExternalInfo externalInfo{};
            externalInfo.metallic = (float)row / (float)NR_ROWS;
            externalInfo.albedo = glm::vec3(0.5f, 0.0f, 0.0f);
            externalInfo.ao = 1.0f;
            for (int col = 0; col < NR_COLUMNS; ++col)
            {
                // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                externalInfo.roughness = glm::clamp((float)col / (float)NR_COLUMNS, 0.05f, 1.0f);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(
                        (float)(col - (NR_COLUMNS / 2)) * SPACING,
                        (float)(row - (NR_ROWS / 2)) * SPACING,
                        -2.0f
                ));
                externalInfo.model = model;
                externalInfo.normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
                int idx = row * NR_COLUMNS + col;
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

                spherePasses[idx].irradianceMapView = irradianceMapView;
                spherePasses[idx].irradianceSampler = irradianceSampler;

                spherePasses[idx].prefilterMapView = prefilterMapView;
                spherePasses[idx].prefilterSampler = prefilterMapSampler;

                spherePasses[idx].brdfLUTView = brdfLUTView;
                spherePasses[idx].brdfLUTSampler = brdfLUTSampler;

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
        createIrradianceResources();
        createIrradianceSampler();
        createPrefilterMapResources();
        createPrefilterSampler();
        createBrdfLUTResources();
        createBrdfLUTSampler();


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

        for(auto & irradianceMapPass : irradianceMapPasses){
            irradianceMapPass.init();
        }

        for(auto & prefilterMapPass : prefilterMapPasses){
            prefilterMapPass.init();
        }

        brdfLUTPass.init();

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

        generateCubemapMipmaps(commandBuffer, envCubemap, CUBEMAP_RESOLUTION, CUBEMAP_RESOLUTION, log2(CUBEMAP_RESOLUTION) + 1, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        //copyImage2DtoCubemap(commandBuffer, colorImages, envCubemap, CUBEMAP_RESOLUTION);

        for(auto& irradianceMapPass : irradianceMapPasses){
            irradianceMapPass.recordCommandBuffer(commandBuffer);
        }

        for(auto& prefilterMapPass : prefilterMapPasses){
            prefilterMapPass.recordCommandBuffer(commandBuffer);
        }

        brdfLUTPass.recordCommandBuffer(commandBuffer);

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

        createImage(IRRADIANCE_MAP_RESOLUTION, IRRADIANCE_MAP_RESOLUTION, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage3, depthImageMemory3);
        depthImageView3 = createImageView(depthImage3, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

        uint32_t resolution = PREFILTER_MAP_RESOLUTION;
        for(uint32_t mipLevel = 0; mipLevel < MIPMAP_COUNT; ++mipLevel){
            for(uint32_t faceIndex = 0; faceIndex < 6; ++faceIndex){
                uint32_t idx = mipLevel * 6 + faceIndex;
                createImage(resolution, resolution, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImages[idx], depthImageMemories[idx]);
                depthImageViews[idx] = createImageView(depthImages[idx], depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
            }
            resolution >>= 1;
        }

        createImage(LUT_RESOLUTION, LUT_RESOLUTION, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage4, depthImageMemory4);
        depthImageView4 = createImageView(depthImage4, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void createEnvResources() {
        VkFormat colorFormat = VK_FORMAT_R8G8B8A8_SRGB;

        createCubemapImage(CUBEMAP_RESOLUTION, CUBEMAP_RESOLUTION, log2(CUBEMAP_RESOLUTION) + 1, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, envCubemap, envCubemapMemory);
        envCubemapView = createCubemapImageView(envCubemap, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        for(uint32_t faceIndex = 0; faceIndex < 6; ++ faceIndex){
            envPerFaceViews[faceIndex] = createFthMthCubemapImageView(envCubemap, colorFormat,
                                                                      VK_IMAGE_ASPECT_COLOR_BIT, faceIndex);
        }
    }

    void createIrradianceResources() {
        VkFormat colorFormat = VK_FORMAT_R8G8B8A8_SRGB;

        createCubemapImage(IRRADIANCE_MAP_RESOLUTION, IRRADIANCE_MAP_RESOLUTION, 1, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, irradianceMap, irradianceMapMemory);
        irradianceMapView = createCubemapImageView(irradianceMap, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        for(uint32_t faceIndex = 0; faceIndex < 6; ++ faceIndex){
            irradiancePerFaceViews[faceIndex] = createFthMthCubemapImageView(irradianceMap, colorFormat,
                                                                             VK_IMAGE_ASPECT_COLOR_BIT, faceIndex);
        }
    }

    void createPrefilterMapResources() {
        VkFormat colorFormat = VK_FORMAT_R8G8B8A8_SRGB;

        createCubemapImage(PREFILTER_MAP_RESOLUTION, PREFILTER_MAP_RESOLUTION, MIPMAP_COUNT, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, prefilterMap, prefilterMapMemory);
        prefilterMapView = createCubemapImageView(prefilterMap, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, MIPMAP_COUNT);

        for(int mipLevel = 0; mipLevel < MIPMAP_COUNT; ++mipLevel){
            for(uint32_t faceIndex = 0; faceIndex < 6; ++ faceIndex){
                prefilterPerFacePerMipViews[mipLevel * 6 + faceIndex] = createFthMthCubemapImageView(prefilterMap, colorFormat,
                                                                                 VK_IMAGE_ASPECT_COLOR_BIT, faceIndex, mipLevel);
            }
        }
    }

    void createBrdfLUTResources(){
        VkFormat colorFormat = VK_FORMAT_R16G16_SFLOAT;
        createImage(LUT_RESOLUTION, LUT_RESOLUTION, 1, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, brdfLUT, brdfLUTMemory);
        brdfLUTView = createImageView(brdfLUT, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
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

    void createBrdfLUTSampler() {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
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

        if (vkCreateSampler(device, &samplerInfo, nullptr, &brdfLUTSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    void createIrradianceSampler() {
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

        if (vkCreateSampler(device, &samplerInfo, nullptr, &irradianceSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    void createPrefilterSampler() {
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

        if (vkCreateSampler(device, &samplerInfo, nullptr, &prefilterMapSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    void createHdrImageView() {
        hdrImageView = createImageView(hdrImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void loadModel(){
        loadBoxModel();
        loadSphereModel();
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

    void loadSphereModel() {
        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;

        // 顶点生成
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
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

        // 索引生成
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
            for (unsigned int x = 0; x < X_SEGMENTS; ++x) {
                // 生成两组三角形（每一对相邻的四个顶点形成两个三角形）
                unsigned int topLeft = y * (X_SEGMENTS + 1) + x;
                unsigned int topRight = y * (X_SEGMENTS + 1) + (x + 1);
                unsigned int bottomLeft = (y + 1) * (X_SEGMENTS + 1) + x;
                unsigned int bottomRight = (y + 1) * (X_SEGMENTS + 1) + (x + 1);

                // 第一个三角形
                indices2.push_back(topLeft);
                indices2.push_back(bottomLeft);
                indices2.push_back(topRight);

                // 第二个三角形
                indices2.push_back(topRight);
                indices2.push_back(bottomLeft);
                indices2.push_back(bottomRight);
            }
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
        createVertexQuadBuffer();
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

    void createVertexQuadBuffer(){
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

        vkDestroyImageView(device, depthImageView3, nullptr);
        vkDestroyImage(device, depthImage3, nullptr);
        vkFreeMemory(device, depthImageMemory3, nullptr);

        vkDestroyImageView(device, depthImageView4, nullptr);
        vkDestroyImage(device, depthImage4, nullptr);
        vkFreeMemory(device, depthImageMemory4, nullptr);

        for(uint32_t mipLevel = 0; mipLevel < MIPMAP_COUNT; ++mipLevel){
            for(uint32_t faceIndex = 0; faceIndex < 6; ++faceIndex){
                uint32_t idx = mipLevel * 6 + faceIndex;
                vkDestroyImageView(device, depthImageViews[idx], nullptr);
                vkDestroyImage(device, depthImages[idx], nullptr);
                vkFreeMemory(device, depthImageMemories[idx], nullptr);
            }
        }

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

        for(auto & irradianceMapPass : irradianceMapPasses){
            for (auto framebuffer : irradianceMapPass.framebuffers) {
                vkDestroyFramebuffer(device, framebuffer, nullptr);
            }
        }

        for(auto & prefilterMapPass : prefilterMapPasses){
            for (auto framebuffer : prefilterMapPass.framebuffers) {
                vkDestroyFramebuffer(device, framebuffer, nullptr);
            }
        }

        for(auto frameBuffer : brdfLUTPass.framebuffers){
            vkDestroyFramebuffer(device, frameBuffer, nullptr);
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

            irradianceMapPasses[faceIndex].cleanup();
            vkDestroyImageView(device, irradiancePerFaceViews[faceIndex], nullptr);
        }

        brdfLUTPass.cleanup();
        skyboxPass.cleanup();
        for(auto& spherePass: spherePasses){
            spherePass.cleanup();
        }

        for(uint32_t mipLevel = 0; mipLevel < MIPMAP_COUNT; ++mipLevel){
            for(uint32_t faceIndex = 0; faceIndex < 6; ++faceIndex){
                uint32_t idx = mipLevel * 6 + faceIndex;
                prefilterMapPasses[idx].cleanup();
                vkDestroyImageView(device, prefilterPerFacePerMipViews[idx], nullptr);
            }
        }


        vkDestroyImageView(device, envCubemapView, nullptr);
        vkDestroyImage(device, envCubemap, nullptr);
        vkFreeMemory(device, envCubemapMemory, nullptr);

        vkDestroyImageView(device, irradianceMapView, nullptr);
        vkDestroyImage(device, irradianceMap, nullptr);
        vkFreeMemory(device, irradianceMapMemory, nullptr);

        vkDestroyImageView(device, prefilterMapView, nullptr);
        vkDestroyImage(device, prefilterMap, nullptr);
        vkFreeMemory(device, prefilterMapMemory, nullptr);

        vkDestroyImageView(device, brdfLUTView, nullptr);
        vkDestroyImage(device, brdfLUT, nullptr);
        vkFreeMemory(device, brdfLUTMemory, nullptr);

        vkDestroySampler(device, hdrSampler, nullptr);
        vkDestroyImageView(device, hdrImageView, nullptr);

        vkDestroyImage(device, hdrImage, nullptr);
        vkFreeMemory(device, hdrImageMemory, nullptr);

        vkDestroySampler(device, envSampler, nullptr);
        vkDestroySampler(device, irradianceSampler, nullptr);
        vkDestroySampler(device, prefilterMapSampler, nullptr);
        vkDestroySampler(device, brdfLUTSampler, nullptr);



        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);

        vkDestroyBuffer(device, indexBuffer2, nullptr);
        vkFreeMemory(device, indexBufferMemory2, nullptr);

        vkDestroyBuffer(device, vertexBuffer2, nullptr);
        vkFreeMemory(device, vertexBufferMemory2, nullptr);

        vkDestroyBuffer(device, vertexBuffer3, nullptr);
        vkFreeMemory(device, vertexBufferMemory3, nullptr);


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
