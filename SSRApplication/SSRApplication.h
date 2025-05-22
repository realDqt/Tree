//
// Created by 22473 on 2025-03-03.
//

#ifndef VKRENDERINGENGINE_SSRAPPLICATION_H
#define VKRENDERINGENGINE_SSRAPPLICATION_H
#include "../BaseApplication/BaseApplication.h"
#include "../Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "SSRutils.h"
#include "BlinnPhongPass.h"
#include "SSRShadowmapPass.h"
#include "GBufferPass.h"
#include "SSRPass.h"

class SSRApplication : public BaseApplication{
public:
    SSRShadowmapPass shadowmapPass;
    GBufferPass gBufferPasses[2];
    SSRPass ssrPass[2];
    BlinnPhongPass blinnPhongPasses[2];

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

    VkSampler gAlbedoSampler;
    VkSampler gWorldPositionSampler;
    VkSampler gWorldNormalSampler;
    VkSampler gDepthSampler;



    void prepareResources() override
    {
        // blinn-phong pass
        // draw cube
        blinnPhongPasses[0].device = device;
        blinnPhongPasses[0].physicalDevice = physicalDevice;

        blinnPhongPasses[0].depthImageView = depthImageView;

        blinnPhongPasses[0].vertexBuffer = vertexBuffer;
        blinnPhongPasses[0].indexBuffer = indexBuffer;
        blinnPhongPasses[0].indicesCount = indices.size();

        blinnPhongPasses[0].swapChainExtent = swapChainExtent;
        blinnPhongPasses[0].swapChainImageFormat = swapChainImageFormat;
        blinnPhongPasses[0].swapChainImageViews = swapChainImageViews;

        blinnPhongPasses[0].textureImageView = textureImageView;
        blinnPhongPasses[0].textureSampler = textureSampler;

        blinnPhongPasses[0].model = cubeModel;
        blinnPhongPasses[0].isFloor = false;
        blinnPhongPasses[0].shadowmapView = shadowmapView;
        blinnPhongPasses[0].smSampler = shadowmapSampler;


        // draw floor
        blinnPhongPasses[1].device = device;
        blinnPhongPasses[1].physicalDevice = physicalDevice;

        blinnPhongPasses[1].depthImageView = depthImageView;

        blinnPhongPasses[1].vertexBuffer = vertexBuffer2;
        blinnPhongPasses[1].indexBuffer = indexBuffer2;
        blinnPhongPasses[1].indicesCount = indices2.size();

        blinnPhongPasses[1].swapChainExtent = swapChainExtent;
        blinnPhongPasses[1].swapChainImageFormat = swapChainImageFormat;
        blinnPhongPasses[1].swapChainImageViews = swapChainImageViews;

        blinnPhongPasses[1].textureImageView = textureImageView;
        blinnPhongPasses[1].textureSampler = textureSampler;

        blinnPhongPasses[1].model = floorModel;
        blinnPhongPasses[1].isFloor = true;
        blinnPhongPasses[1].shadowmapView = shadowmapView;
        blinnPhongPasses[1].smSampler = shadowmapSampler;


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
        gBufferPasses[0].gDepthView = gDepthView;

        gBufferPasses[0].model = cubeModel;
        gBufferPasses[0].isFloor = false;


        gBufferPasses[1].device = device;
        gBufferPasses[1].physicalDevice = physicalDevice;

        gBufferPasses[1].swapChainExtent = swapChainExtent;
        gBufferPasses[1].swapChainImageViewCount = swapChainImageViews.size();

        gBufferPasses[1].depthImageView = depthImageView;

        gBufferPasses[1].vertexBuffer = vertexBuffer;
        gBufferPasses[1].indexBuffer = indexBuffer;
        gBufferPasses[1].indicesCount = indices.size();

        gBufferPasses[1].textureImageView = textureImageView;
        gBufferPasses[1].textureSampler = textureSampler;

        gBufferPasses[1].gAlbedoView = gAlbedoView;
        gBufferPasses[1].gWorldPositionView = gWorldPositionView;
        gBufferPasses[1].gWorldNormalView = gWorldNormalView;
        gBufferPasses[1].gDepthView = gDepthView;

        gBufferPasses[1].model = floorModel;
        gBufferPasses[1].isFloor = true;


        // ssr pass
        ssrPass[0].device = device;
        ssrPass[0].physicalDevice = physicalDevice;

        ssrPass[0].swapChainExtent = swapChainExtent;
        ssrPass[0].swapChainImageFormat = swapChainImageFormat;
        ssrPass[0].swapChainImageViews = swapChainImageViews;

        ssrPass[0].depthImageView = depthImageView;

        ssrPass[0].vertexBuffer = vertexBuffer;
        ssrPass[0].indexBuffer = indexBuffer;
        ssrPass[0].indicesCount = indices.size();

        ssrPass[0].gAlbedoImageView = gAlbedoView;
        ssrPass[0].gAlbedoSampler = gAlbedoSampler;

        ssrPass[0].gWorldPositionImageView = gWorldPositionView;
        ssrPass[0].gWorldPositionSampler = gWorldPositionSampler;

        ssrPass[0].gWorldNormalImageView = gWorldNormalView;
        ssrPass[0].gWorldNormalSampler = gWorldNormalSampler;

        ssrPass[0].gDepthImageView = gDepthView;
        ssrPass[0].gDepthSampler = gDepthSampler;

        ssrPass[0].smImageView = shadowmapView;
        ssrPass[0].smSampler = shadowmapSampler;

        ssrPass[0].model = cubeModel;

        ssrPass[1].device = device;
        ssrPass[1].physicalDevice = physicalDevice;

        ssrPass[1].swapChainExtent = swapChainExtent;
        ssrPass[1].swapChainImageFormat = swapChainImageFormat;
        ssrPass[1].swapChainImageViews = swapChainImageViews;

        ssrPass[1].depthImageView = depthImageView;

        ssrPass[1].vertexBuffer = vertexBuffer2;
        ssrPass[1].indexBuffer = indexBuffer2;
        ssrPass[1].indicesCount = indices2.size();

        ssrPass[1].gAlbedoImageView = gAlbedoView;
        ssrPass[1].gAlbedoSampler = gAlbedoSampler;

        ssrPass[1].gWorldPositionImageView = gWorldPositionView;
        ssrPass[1].gWorldPositionSampler = gWorldPositionSampler;

        ssrPass[1].gWorldNormalImageView = gWorldNormalView;
        ssrPass[1].gWorldNormalSampler = gWorldNormalSampler;

        ssrPass[1].gDepthImageView = gDepthView;
        ssrPass[1].gDepthSampler = gDepthSampler;

        ssrPass[1].smImageView = shadowmapView;
        ssrPass[1].smSampler = shadowmapSampler;

        ssrPass[1].model = floorModel;


    }
    void initVulkan() override{
        //camera.Position = lightPos;
        BaseApplication::initVulkan();
        createColorResources();
        createDepthResources();
        createGBufferResources();

        createTextureImage();
        createTextureImageView();
        createTextureSampler();

        createShadowmapSampler();
        createGSampler(gAlbedoSampler);
        createGSampler(gWorldPositionSampler);
        createGSampler(gWorldNormalSampler);
        createGSampler(gDepthSampler);


        loadModel();
        createVertexBuffer();
        createIndexBuffer();

        prepareResources();
        shadowmapPass.init();
        blinnPhongPasses[0].init();
        blinnPhongPasses[1].init();
        gBufferPasses[0].init();
        gBufferPasses[1].init();
        ssrPass[0].init();
        ssrPass[1].init();
    }

    void cleanupSwapChain() override{

        vkDestroyImageView(device, colorImageView, nullptr);
        vkFreeMemory(device, colorImageMemory, nullptr);
        vkDestroyImage(device, colorImage, nullptr);

        vkDestroyImageView(device, depthImageView, nullptr);
        vkFreeMemory(device, depthImageMemory, nullptr);
        vkDestroyImage(device, depthImage, nullptr);

        vkDestroyImageView(device, depthImageView2, nullptr);
        vkFreeMemory(device, depthImageMemory2, nullptr);
        vkDestroyImage(device, depthImage2, nullptr);

        for(auto& framebuffer : shadowmapPass.framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for(auto& framebuffer : blinnPhongPasses[0].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for(auto& framebuffer : blinnPhongPasses[1].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for(auto& framebuffer :gBufferPasses[0].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for(auto& framebuffer : gBufferPasses[1].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for(auto& framebuffer :ssrPass[0].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for(auto& framebuffer : ssrPass[1].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    void cleanup() override{
        cleanupSwapChain();
        shadowmapPass.cleanup();
        blinnPhongPasses[0].cleanup();
        blinnPhongPasses[1].cleanup();

        gBufferPasses[0].cleanup();
        gBufferPasses[1].cleanup();

        ssrPass[0].cleanup();
        ssrPass[1].cleanup();

        vkDestroyImageView(device, gAlbedoView, nullptr);
        vkFreeMemory(device, gAlbedoMemory, nullptr);
        vkDestroyImage(device, gAlbedo, nullptr);
        vkDestroySampler(device, gAlbedoSampler, nullptr);

        vkDestroyImageView(device, gWorldPositionView, nullptr);
        vkFreeMemory(device, gWorldPositionMemory, nullptr);
        vkDestroyImage(device, gWorldPosition, nullptr);
        vkDestroySampler(device, gWorldPositionSampler, nullptr);

        vkDestroyImageView(device, gWorldNormalView, nullptr);
        vkFreeMemory(device, gWorldNormalMemory, nullptr);
        vkDestroyImage(device, gWorldNormal, nullptr);
        vkDestroySampler(device, gWorldNormalSampler, nullptr);

        vkDestroyImageView(device, gDepthView, nullptr);
        vkFreeMemory(device, gDepthMemory, nullptr);
        vkDestroyImage(device, gDepth, nullptr);
        vkDestroySampler(device, gDepthSampler, nullptr);

        vkDestroyImageView(device, shadowmapView, nullptr);
        vkFreeMemory(device, shadowmapMemory, nullptr);
        vkDestroyImage(device, shadowmap, nullptr);
        vkDestroySampler(device, shadowmapSampler, nullptr);

        vkDestroyImageView(device, textureImageView, nullptr);
        vkFreeMemory(device, textureImageMemory, nullptr);
        vkDestroyImage(device, textureImage, nullptr);
        vkDestroySampler(device, textureSampler, nullptr);

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);

        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);

        vkDestroyBuffer(device, vertexBuffer2, nullptr);
        vkFreeMemory(device, vertexBufferMemory2, nullptr);

        vkDestroyBuffer(device, indexBuffer2, nullptr);
        vkFreeMemory(device, indexBufferMemory2, nullptr);

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

    void createVertexBuffer(){
        createCubeVertexBuffer();
        createFloorVertexBuffer();
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
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;  // 使用线性过滤
        samplerInfo.minFilter = VK_FILTER_LINEAR;  // 使用线性过滤
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;  // 边界模式
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;  // 边界颜色为白色
        samplerInfo.anisotropyEnable = VK_FALSE;  // 关闭各向异性过滤
        samplerInfo.maxAnisotropy = 1.0f;  // 设置为 1.0（无效，但需要初始化）
        samplerInfo.unnormalizedCoordinates = VK_FALSE;  // 使用标准化坐标
        samplerInfo.compareEnable = VK_TRUE;  // 启用深度比较
        samplerInfo.compareOp = VK_COMPARE_OP_LESS;  // 深度比较操作
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;  // 禁用 Mipmap
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;  // 禁用 Mipmap
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &shadowmapSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shadow map sampler!");
        }
    }

    void createGSampler(VkSampler& gSampler) {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;  // 使用线性过滤
        samplerInfo.minFilter = VK_FILTER_LINEAR;  // 使用线性过滤
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;  // 边界模式
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;  // 边界颜色为白色
        samplerInfo.anisotropyEnable = VK_FALSE;  // 关闭各向异性过滤
        samplerInfo.maxAnisotropy = 1.0f;  // 设置为 1.0（无效，但需要初始化）
        samplerInfo.unnormalizedCoordinates = VK_FALSE;  // 使用标准化坐标
        samplerInfo.compareEnable = VK_TRUE;  // 启用深度比较
        samplerInfo.compareOp = VK_COMPARE_OP_LESS;  // 深度比较操作
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;  // 禁用 Mipmap
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;  // 禁用 Mipmap
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &gSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shadow map sampler!");
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
    }

    void loadModel(){
        loadCube();
        /*
        for(auto& vertex : vertices){
            std::cout << "pos: (" << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z <<")"<< std::endl;
            std::cout << "normal: (" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << std::endl;
            std::cout << "texCoord: (" << vertex.texCoord.x << ", " << vertex.texCoord.y << std::endl;
            std::cout << "-------------------------------------------------" << std::endl;
        }

        for(auto& index : indices){
            std::cout << index << std::endl;
        }
         */
        loadFloor();

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

        colorFormat = VK_FORMAT_R8G8B8A8_SRGB;
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

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, gDepthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, gDepth, gDepthMemory);
        gDepthView = createImageView(gDepth, gDepthFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

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

        //blinnPhongPasses[0].recordCommandBuffer(commandBuffer, imageIndex);
        //blinnPhongPasses[1].recordCommandBuffer(commandBuffer, imageIndex);
        ssrPass[0].recordCommandBuffer(commandBuffer, imageIndex);
        ssrPass[1].recordCommandBuffer(commandBuffer, imageIndex);


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
        blinnPhongPasses[0].currentFrame = currentFrame;
        blinnPhongPasses[1].currentFrame = currentFrame;
        shadowmapPass.currentFrame = currentFrame;
        gBufferPasses[0].currentFrame = currentFrame;
        gBufferPasses[1].currentFrame = currentFrame;
        ssrPass[0].currentFrame = currentFrame;
        ssrPass[1].currentFrame = currentFrame;
    }
};

#endif //VKRENDERINGENGINE_SSRAPPLICATION_H
