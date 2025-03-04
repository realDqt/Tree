//
// Created by 22473 on 2025-03-03.
//

#ifndef VKRENDERINGENGINE_SSRAPPLICATION_H
#define VKRENDERINGENGINE_SSRAPPLICATION_H
#include "../BaseApplication/BaseApplication.h"
#include "../Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "SSRutils.h"

class SSRApplication : public BaseApplication{
public:


    void prepareResources() override
    {

    }

    void initVulkan() override{
        //camera.Position = lightPos;
        BaseApplication::initVulkan();
    }

    void cleanupSwapChain() override{
        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    void cleanup() override{
        cleanupSwapChain();


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
    }

    // ------------------begin to test assimp------------------

    // 递归遍历场景中的节点
    void ProcessNode(aiNode* node, const aiScene* scene) {
        std::cout << "Processing node: " << node->mName.C_Str() << std::endl;

        // 遍历当前节点的所有网格
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            std::cout << "  Mesh name: " << mesh->mName.C_Str() << std::endl;
            std::cout << "  Number of vertices: " << mesh->mNumVertices << std::endl;

            // 如果需要，可以在这里处理顶点、法线、纹理坐标等
            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D vertex = mesh->mVertices[j];
                std::cout << "    Vertex " << j << ": (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
            }
        }

        // 递归处理子节点
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    void run()override{

        // 设置要加载的 GLTF 文件路径
        std::string filePath = CUBE_PATH;

        // 创建 Assimp 导入器
        Assimp::Importer importer;
        // 加载 GLTF 文件
        const aiScene* scene = importer.ReadFile(filePath,
                                                 aiProcess_Triangulate |        // 将所有多边形转为三角形
                                                 aiProcess_FlipUVs |           // 翻转纹理坐标（如果需要）
                                                 aiProcess_JoinIdenticalVertices // 合并相同顶点
        );

        // 检查文件是否加载成功
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Error loading GLTF file: " << importer.GetErrorString() << std::endl;
        }

        // 输出场景信息
        std::cout << "Scene loaded successfully!" << std::endl;
        std::cout << "Number of meshes: " << scene->mNumMeshes << std::endl;
        std::cout << "Number of materials: " << scene->mNumMaterials << std::endl;

        // 递归处理场景中的节点
        ProcessNode(scene->mRootNode, scene);
    }

    // ------------------end to test assimp------------------

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }


        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

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
    }
};

#endif //VKRENDERINGENGINE_SSRAPPLICATION_H
