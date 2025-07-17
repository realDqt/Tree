//
// Created by 22473 on 2025-01-20.
//
#ifndef VKRENDERINGENGINE_PCSSAPPLICATION_H
#define VKRENDERINGENGINE_PCSSAPPLICATION_H
#include "../BaseApplication/BaseApplication.h"
#include "ShadowmapPass.h"
#include "BlinPhongPass.h"
#include "LightVisualizationPass.h"
#include "../EngineCore/Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

class PCSSApplication : public BaseApplication{
public:

    ShadowmapPass shadowmapPass;
    BlinPhongPass blinPhongPass;
    LightVisualizationPass lightVisualizationPass;

    // blin-phong
    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    // shadow map
    VkImage SMcolorImage;
    VkDeviceMemory SMcolorImageMemory;
    VkImageView SMcolorImageView;

    VkSampler SMSampler;

    VkImage SMdepthImage;
    VkDeviceMemory SMdepthImageMemory;
    VkImageView SMdepthImageView;

    uint32_t mipLevels;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    // vb and ib for marry
    std::vector<VertexMarry> vertices;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    // vb and ib for floor
    std::vector<VertexMarry> vertices2;
    std::vector<uint32_t> indices2;
    VkBuffer vertexBuffer2;
    VkDeviceMemory vertexBufferMemory2;
    VkBuffer indexBuffer2;
    VkDeviceMemory indexBufferMemory2;

    // vb and ib for light
    std::vector<VertexLight> vertices3;
    std::vector<uint32_t> indices3;
    VkBuffer vertexBuffer3;
    VkDeviceMemory vertexBufferMemory3;
    VkBuffer indexBuffer3;
    VkDeviceMemory indexBufferMemory3;


    void prepareResources() override
    {
        // shadow-map pass
        shadowmapPass.device = device;
        shadowmapPass.physicalDevice = physicalDevice;
        shadowmapPass.currentFrame = currentFrame;
        shadowmapPass.swapChainImagesCount = swapChainImageViews.size();


        shadowmapPass.colorImageView = SMcolorImageView;
        shadowmapPass.depthImageView = SMdepthImageView;

        shadowmapPass.vertexBufferMarry = vertexBuffer;
        shadowmapPass.indexBufferMarry = indexBuffer;
        shadowmapPass.indicesCountMarry = indices.size();

        shadowmapPass.vertexBufferFloor = vertexBuffer2;
        shadowmapPass.indexBufferFloor = indexBuffer2;
        shadowmapPass.indicesCountFloor = indices2.size();

        // blin-phong pass
        blinPhongPass.device = device;
        blinPhongPass.physicalDevice = physicalDevice;
        blinPhongPass.currentFrame = currentFrame;
        blinPhongPass.msaaSamples = msaaSamples;

        blinPhongPass.swapChainImageFormat = swapChainImageFormat;
        blinPhongPass.swapChainExtent = swapChainExtent;

        blinPhongPass.swapChainImageViews.resize(swapChainImageViews.size());
        for(size_t i = 0; i < swapChainImageViews.size(); ++i)
            blinPhongPass.swapChainImageViews[i] = swapChainImageViews[i];


        blinPhongPass.colorImageView = colorImageView;
        blinPhongPass.depthImageView = depthImageView;

        blinPhongPass.vertexBufferMarry = vertexBuffer;
        blinPhongPass.indexBufferMarry = indexBuffer;
        blinPhongPass.indicesCountMarry = indices.size();

        blinPhongPass.vertexBufferFloor = vertexBuffer2;
        blinPhongPass.indexBufferFloor = indexBuffer2;
        blinPhongPass.indicesCountFloor = indices2.size();

        blinPhongPass.SMcolorImageView = SMcolorImageView;
        blinPhongPass.SMSampler = SMSampler;
        blinPhongPass.textureImageView = textureImageView;
        blinPhongPass.textureSampler = textureSampler;

        // light visualization pass
        lightVisualizationPass.device = device;
        lightVisualizationPass.physicalDevice = physicalDevice;
        lightVisualizationPass.currentFrame = currentFrame;
        lightVisualizationPass.msaaSamples = msaaSamples;

        lightVisualizationPass.swapChainImageFormat = swapChainImageFormat;
        lightVisualizationPass.swapChainExtent = swapChainExtent;

        lightVisualizationPass.swapChainImageViews.resize(swapChainImageViews.size());
        for(size_t i = 0; i < swapChainImageViews.size(); ++i)
            lightVisualizationPass.swapChainImageViews[i] = swapChainImageViews[i];


        lightVisualizationPass.colorImageView = colorImageView;
        lightVisualizationPass.depthImageView = depthImageView;

        lightVisualizationPass.vertexBuffer = vertexBuffer3;
        lightVisualizationPass.indexBuffer = indexBuffer3;
        lightVisualizationPass.indicesCount = indices3.size();
    }

    void initVulkan() override{
        //camera.Position = lightPos;
        BaseApplication::initVulkan();

        createColorResources();
        createSMColorResources();
        createDepthResources();
        createSMDepthResources();
        createTextureImage();
        createTextureImageView();
        createTextureSampler();
        createSMSampler();
        loadModel();
        createVertexBuffer();
        createIndexBuffer();

        prepareResources();
        shadowmapPass.init();
        blinPhongPass.init();
        lightVisualizationPass.init();
    }

    void cleanupSwapChain() override{
        vkDestroyImageView(device, depthImageView, nullptr);
        vkDestroyImage(device, depthImage, nullptr);
        vkFreeMemory(device, depthImageMemory, nullptr);

        vkDestroyImageView(device, colorImageView, nullptr);
        vkDestroyImage(device, colorImage, nullptr);
        vkFreeMemory(device, colorImageMemory, nullptr);

        for(auto& framebuffer : shadowmapPass.framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for (auto framebuffer : blinPhongPass.framebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for (auto framebuffer : lightVisualizationPass.framebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    void cleanup() override{
        cleanupSwapChain();

        vkDestroyImageView(device, SMdepthImageView, nullptr);
        vkDestroyImage(device, SMdepthImage, nullptr);
        vkFreeMemory(device, SMdepthImageMemory, nullptr);

        vkDestroyImageView(device, SMcolorImageView, nullptr);
        vkDestroyImage(device, SMcolorImage, nullptr);
        vkFreeMemory(device, SMcolorImageMemory, nullptr);

        shadowmapPass.cleanup();
        blinPhongPass.cleanup();
        lightVisualizationPass.cleanup();

        vkDestroySampler(device, textureSampler, nullptr);
        vkDestroyImageView(device, textureImageView, nullptr);

        vkDestroyImage(device, textureImage, nullptr);
        vkFreeMemory(device, textureImageMemory, nullptr);

        vkDestroySampler(device, SMSampler, nullptr);


        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);


        vkDestroyBuffer(device, indexBuffer2, nullptr);
        vkFreeMemory(device, indexBufferMemory2, nullptr);

        vkDestroyBuffer(device, vertexBuffer2, nullptr);
        vkFreeMemory(device, vertexBufferMemory2, nullptr);

        vkDestroyBuffer(device, indexBuffer3, nullptr);
        vkFreeMemory(device, indexBufferMemory3, nullptr);

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

        createSMColorResources();
        createSMDepthResources();
    }


    void createColorResources() {
        VkFormat colorFormat = swapChainImageFormat;

        createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
        colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void createSMColorResources(){
        VkFormat colorFormat = VK_FORMAT_R8G8B8A8_SRGB;

        createImage(SMResolution, SMResolution, 1, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, SMcolorImage, SMcolorImageMemory);
        SMcolorImageView = createImageView(SMcolorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void createDepthResources() {
        VkFormat depthFormat = findDepthFormat();

        createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
        depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void createSMDepthResources() {
        VkFormat depthFormat = findDepthFormat();

        createImage(SMResolution, SMResolution, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, SMdepthImage, SMdepthImageMemory);
        SMdepthImageView = createImageView(SMdepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void createTextureImage() {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
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


    void createTextureImageView() {
        textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
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

    void createSMSampler() {
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

        if (vkCreateSampler(device, &samplerInfo, nullptr, &SMSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shadow map sampler!");
        }
    }

    void loadModel() {
        loadMarry();
        loadFloor();
        loadLight();
    }

    void loadLight()
    {
        float positions[] = {
                // 后面 (z = -0.5)
                -0.5f, -0.5f, -0.5f, // v0
                0.5f, -0.5f, -0.5f, // v1
                0.5f,  0.5f, -0.5f, // v2
                -0.5f,  0.5f, -0.5f, // v3

                // 前面 (z = 0.5)
                -0.5f, -0.5f,  0.5f, // v4
                0.5f, -0.5f,  0.5f, // v5
                0.5f,  0.5f,  0.5f, // v6
                -0.5f,  0.5f,  0.5f, // v7
        };

        vertices3.clear();
        for(int i = 0; i < 8; ++i){
            VertexLight vertex{};
            vertex.pos = glm::vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);
            vertices3.push_back(vertex);
        }

        indices3.clear();
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
            indices3.push_back(id);
    }

    void loadMarry()
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MARRY_PATH.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<VertexMarry, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                VertexMarry vertex{};

                vertex.pos = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                };

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);


                /*
                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
                 */
            }
        }
    }

    void loadFloor()
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, FLOOR_PATH.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<VertexMarry, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                VertexMarry vertex{};

                vertex.pos = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                };

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices2.size());
                    vertices2.push_back(vertex);
                }

                indices2.push_back(uniqueVertices[vertex]);


                /*
                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
                 */
            }
        }
    }

    void createVertexBuffer() {
        createMarryVertexBuffer();
        createFloorVertexBuffer();
        createLightVertexBuffer();
    }

    void createLightVertexBuffer()
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

    void createMarryVertexBuffer()
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

    void createIndexBuffer() {
        createMarryIndexBuffer();
        createFloorIndexBuffer();
        createLVIndexBuffer();
    }

    void createLVIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(indices3[0]) * indices3.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices3.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer3, indexBufferMemory3);

        copyBuffer(stagingBuffer, indexBuffer3, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void createMarryIndexBuffer()
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

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        // shadow map pass
        shadowmapPass.recordCommandBuffer(commandBuffer, imageIndex);

        // blin-phong pass
        blinPhongPass.recordCommandBuffer(commandBuffer, imageIndex);

        // Light visualization pass
        lightVisualizationPass.recordCommandBuffer(commandBuffer, imageIndex);

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
        lightVisualizationPass.currentFrame = blinPhongPass.currentFrame = shadowmapPass.currentFrame = currentFrame;
    }
};
#endif //VKRENDERINGENGINE_PCSSAPPLICATION_H
