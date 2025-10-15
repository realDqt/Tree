//
// Created by Administrator on 2025/10/13.
//

#ifndef VKRENDERINGENGINE_SSAOAPPLICATION_H
#define VKRENDERINGENGINE_SSAOAPPLICATION_H
#include "../BaseApplication/BaseApplication.h"
#include "SSAOutils.h"
#include <random>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "SSAOGBufferPass.h"
#include "SSAOPass.h"
#include "BlinPhongPassSSAO.h"

class SSAOApplication : public BaseApplication{
public:
    SSAOGBufferPass gBufferPasses[2];
    SSAOPass ssaoPass;
    BlinPhongPassSSAO blinPhongPasses[2];

    // vb and ib for robot
    std::vector<VertexMarry> vertices;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    glm::mat4 robotModel;

    // vb and ib for floor
    std::vector<VertexMarry> vertices2;
    std::vector<uint32_t> indices2;
    VkBuffer vertexBuffer2;
    VkDeviceMemory vertexBufferMemory2;
    VkBuffer indexBuffer2;
    VkDeviceMemory indexBufferMemory2;
    glm::mat4 floorModel;

    // vb for full-quad screen
    std::vector<VertexQuad> verticesQuad;
    VkBuffer vertexBufferQuad;
    VkDeviceMemory vertexBufferMemoryQuad;


    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    // not use?
    VkImage depthImage2;
    VkDeviceMemory depthImageMemory2;
    VkImageView depthImageView2;

    // resources for gBuffer
    VkImage gAlbedo;
    VkDeviceMemory gAlbedoMemory;
    VkImageView gAlbedoView;

    VkImage gViewPosition;
    VkDeviceMemory gViewPositionMemory;
    VkImageView gViewPositionView;

    VkImage gViewNormal;
    VkDeviceMemory gViewNormalMemory;
    VkImageView gViewNormalView;

    VkImage gDepth;
    VkDeviceMemory gDepthMemory;
    VkImageView gDepthView;

    VkImage occlusionImage;
    VkDeviceMemory occlusionMemory;
    VkImageView occlusionView;

    NoiseTexture noiseTexture;

    VkSampler gAlbedoSampler;
    VkSampler gViewPositionSampler;
    VkSampler gViewNormalSampler;
    VkSampler gDepthSampler;

    void prepareResources() override{
        // GBuffer Passes
        gBufferPasses[0].device = device;
        gBufferPasses[0].physicalDevice = physicalDevice;

        gBufferPasses[0].swapChainExtent = swapChainExtent;
        gBufferPasses[0].swapChainImageViewCount = swapChainImageViews.size();

        gBufferPasses[0].depthImageView = depthImageView;

        gBufferPasses[0].vertexBuffer = vertexBuffer;
        gBufferPasses[0].indexBuffer = indexBuffer;
        gBufferPasses[0].indicesCount = indices.size();

        gBufferPasses[0].gAlbedoView = gAlbedoView;
        gBufferPasses[0].gViewPositionView = gViewPositionView;
        gBufferPasses[0].gViewNormalView = gViewNormalView;
        gBufferPasses[0].gDepthView = gDepthView;

        gBufferPasses[0].model = robotModel;
        gBufferPasses[0].isFloor = false;

        gBufferPasses[0].currentFrame = currentFrame;

        // SSAO Pass
        ssaoPass.device = device;
        ssaoPass.physicalDevice = physicalDevice;

        ssaoPass.swapChainExtent = swapChainExtent;
        ssaoPass.swapChainImageFormat = swapChainImageFormat;
        ssaoPass.swapChainImageViews = swapChainImageViews;
        ssaoPass.swapChainImageViewCount = swapChainImageViews.size();

        ssaoPass.depthImageView = depthImageView;

        ssaoPass.vertexBuffer = vertexBufferQuad;
        ssaoPass.indexBuffer = VkBuffer{};
        ssaoPass.indicesCount = 0;

        ssaoPass.gViewPositionView = gViewPositionView;
        ssaoPass.gViewPositionSampler = gViewPositionSampler;

        ssaoPass.gViewNormalView = gViewNormalView;
        ssaoPass.gViewNormalSampler = gViewNormalSampler;

        ssaoPass.gDepthView = gDepthView;
        ssaoPass.gDepthSampler = gDepthSampler;

        ssaoPass.texNoiseView = noiseTexture.view;
        ssaoPass.texNoiseSampler = noiseTexture.sampler;

        ssaoPass.occlusionView = occlusionView;

        ssaoPass.currentFrame = currentFrame;

        // blin-phong pass for testing
        blinPhongPasses[0].device = device;
        blinPhongPasses[0].physicalDevice = physicalDevice;

        blinPhongPasses[0].depthImageView = depthImageView;

        blinPhongPasses[0].vertexBuffer = vertexBuffer;
        blinPhongPasses[0].indexBuffer = indexBuffer;
        blinPhongPasses[0].indicesCount = indices.size();

        blinPhongPasses[0].swapChainImageViews = swapChainImageViews;
        blinPhongPasses[0].swapChainExtent = swapChainExtent;
        blinPhongPasses[0].swapChainImageFormat = swapChainImageFormat;

        blinPhongPasses[0].currentFrame = currentFrame;

        robotModel = glm::mat4(1.0f);
        robotModel = glm::translate(robotModel, glm::vec3(0.0f, 0.1f, 0.0));
        robotModel = glm::rotate(robotModel, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        robotModel = glm::scale(robotModel, glm::vec3(0.5f));
        blinPhongPasses[0].model = robotModel;

        blinPhongPasses[1].device = device;
        blinPhongPasses[1].physicalDevice = physicalDevice;

        blinPhongPasses[1].depthImageView = depthImageView;

        blinPhongPasses[1].vertexBuffer = vertexBuffer2;
        blinPhongPasses[1].indexBuffer = indexBuffer2;
        blinPhongPasses[1].indicesCount = indices2.size();

        blinPhongPasses[1].swapChainImageViews = swapChainImageViews;
        blinPhongPasses[1].swapChainExtent = swapChainExtent;
        blinPhongPasses[1].swapChainImageFormat = swapChainImageFormat;

        blinPhongPasses[1].currentFrame = currentFrame;

        floorModel = glm::mat4(1.0f);
        floorModel = glm::translate(floorModel, glm::vec3(0.0, -1.0f, 0.0f));
        floorModel = glm::scale(floorModel, glm::vec3(20.0f, 1.0f, 20.0f));
        blinPhongPasses[1].model = floorModel;
    }

    void checkValid() override
    {
        assert(gBufferPasses[0].IsValid());
        assert(ssaoPass.IsValid());
        assert(blinPhongPasses[0].IsValid());
        assert(blinPhongPasses[1].IsValid());
    }

    void initVulkan() override{
        //camera.Position = lightPos;
        BaseApplication::initVulkan();
        createColorResources();
        createDepthResources();
        createGBufferResources();
        createOcclusionResources();


        createGSampler(gAlbedoSampler);
        createGSampler(gViewPositionSampler);
        createGSampler(gViewNormalSampler);
        createGSampler(gDepthSampler);


        loadModel();
        createVertexBuffer();
        createIndexBuffer();

        createSSAOJitterNoiseTexture();

        prepareResources();
        gBufferPasses[0].init();
        //gBufferPasses[1].init();
        ssaoPass.init();
        blinPhongPasses[0].init();
        blinPhongPasses[1].init();
        checkValid();
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


        for(auto& framebuffer :gBufferPasses[0].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        //for(auto& framebuffer : gBufferPasses[1].framebuffers){
           //vkDestroyFramebuffer(device, framebuffer, nullptr);
        //}

        for(auto& framebuffer :ssaoPass.framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for(auto& framebuffer :blinPhongPasses[0].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for(auto& framebuffer :blinPhongPasses[1].framebuffers){
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    void cleanup() override {
        cleanupSwapChain();

        gBufferPasses[0].cleanup();
        //gBufferPasses[1].cleanup();
        ssaoPass.cleanup();
        blinPhongPasses[0].cleanup();
        blinPhongPasses[1].cleanup();

        DestroyNoiseTexture();

        vkDestroyImageView(device, gAlbedoView, nullptr);
        vkFreeMemory(device, gAlbedoMemory, nullptr);
        vkDestroyImage(device, gAlbedo, nullptr);
        vkDestroySampler(device, gAlbedoSampler, nullptr);

        vkDestroyImageView(device, gViewPositionView, nullptr);
        vkFreeMemory(device, gViewPositionMemory, nullptr);
        vkDestroyImage(device, gViewPosition, nullptr);
        vkDestroySampler(device, gViewPositionSampler, nullptr);

        vkDestroyImageView(device, gViewNormalView, nullptr);
        vkFreeMemory(device, gViewNormalMemory, nullptr);
        vkDestroyImage(device, gViewNormal, nullptr);
        vkDestroySampler(device, gViewNormalSampler, nullptr);

        vkDestroyImageView(device, gDepthView, nullptr);
        vkFreeMemory(device, gDepthMemory, nullptr);
        vkDestroyImage(device, gDepth, nullptr);
        vkDestroySampler(device, gDepthSampler, nullptr);

        vkDestroyImageView(device, occlusionView, nullptr);
        vkFreeMemory(device, occlusionMemory, nullptr);
        vkDestroyImage(device, occlusionImage, nullptr);


        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);

        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);

        vkDestroyBuffer(device, vertexBuffer2, nullptr);
        vkFreeMemory(device, vertexBufferMemory2, nullptr);

        vkDestroyBuffer(device, indexBuffer2, nullptr);
        vkFreeMemory(device, indexBufferMemory2, nullptr);

        vkDestroyBuffer(device, vertexBufferQuad, nullptr);
        vkFreeMemory(device, vertexBufferMemoryQuad, nullptr);

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
        createRobotVertexBuffer();
        createFloorVertexBuffer();
        createQuadVertexBuffer();
    }

    void createIndexBuffer(){
        createRobotIndexBuffer();
        createFloorIndexBuffer();
    }

    void createQuadVertexBuffer(){
        VkDeviceSize bufferSize = sizeof(verticesQuad[0]) * verticesQuad.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, verticesQuad.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferQuad, vertexBufferMemoryQuad);

        copyBuffer(stagingBuffer, vertexBufferQuad, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
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

        verticesQuad.clear();
        for(uint32_t i = 0; i < 4; ++i){
            VertexQuad vertexQuad{};
            vertexQuad.pos = glm::vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);
            vertexQuad.texCoord = glm::vec2(texCoords[i * 2], texCoords[i * 2 + 1]);

            verticesQuad.push_back(vertexQuad);
        }
    }

    void loadFloor(){
        float verticesCube[] = {
                // positions          // normals           // texture coords
                // Back face
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // 0
                0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // 1
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // 2
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // 3
                // Front face
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // 4
                0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, // 5
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // 6
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, // 7
                // Left face
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 8
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 9
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 10
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 11
                // Right face
                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 12
                0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 13
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 14
                0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 15
                // Bottom face
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // 16
                0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // 17
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // 18
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // 19
                // Top face
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // 20
                0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // 21
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // 22
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f  // 23
        };

        // Indices for the 12 triangles (2 per face)
        uint32_t indicesCube[] = {
                // Back face
                0, 1, 2,  2, 3, 0,
                // Front face
                4, 5, 6,  6, 7, 4,
                // Left face
                8, 9, 10, 10, 11, 8,
                // Right face (**** CORRECTED ****)
                12, 15, 14, 12, 14, 13,
                // Bottom face
                16, 17, 18, 18, 19, 16,
                // Top face (**** CORRECTED ****)
                20, 23, 22, 22, 21, 20
        };

        // vertices2
        vertices2.clear();
        for(uint32_t i = 0; i < 24u; ++i){
            VertexMarry vertex{};
            vertex.pos = glm::vec3(verticesCube[i * 8], verticesCube[i * 8 + 1], verticesCube[i * 8 + 2]);
            vertex.normal = glm::vec3(verticesCube[i * 8 + 3], verticesCube[i * 8 + 4], verticesCube[i * 8 + 5]);
            vertex.texCoord = glm::vec2(verticesCube[i * 8 + 6], verticesCube[i * 8 + 7]);
            vertices2.push_back(vertex);
        }

        // indices2
        indices2.clear();
        for(uint32_t i = 0; i < 36u; ++i){
            indices2.push_back(indicesCube[i]);
        }
    }

    void createFloorVertexBuffer(){
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

    void createFloorIndexBuffer(){
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

    void createRobotVertexBuffer(){
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

    void createRobotIndexBuffer(){
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

    void loadMarry()
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MARRY_SSAO_PATH.c_str())) {
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

    void loadRobot()
    {
        Assimp::Importer imp;
        const aiScene* scene = imp.ReadFile(
                ROBOT_PATH,
                aiProcess_Triangulate |
                aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            throw std::runtime_error("loadObj: " + std::string(imp.GetErrorString()));

        vertices.clear();
        indices.clear();

        // .obj 里所有 mesh 都在 scene->mMeshes[]，直接遍历
        for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
        {
            const aiMesh* mesh = scene->mMeshes[m];

            // 顶点
            const auto baseVertex = static_cast<uint32_t>(vertices.size());
            for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
            {
                VertexMarry vert{};
                vert.pos      = aiVec2glmVec(mesh->mVertices[v]);
                vert.normal   = mesh->HasNormals()
                                ? aiVec2glmVec(mesh->mNormals[v])
                                : glm::vec3(0.0f);
                vert.texCoord = mesh->HasTextureCoords(0)
                                ? glm::vec2(mesh->mTextureCoords[0][v].x,
                                            mesh->mTextureCoords[0][v].y)
                                : glm::vec2(0.0f);
                vertices.push_back(vert);
            }

            // 索引
            for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
            {
                const aiFace& face = mesh->mFaces[f];
                for (unsigned int i = 0; i < face.mNumIndices; ++i)
                    indices.push_back(baseVertex + face.mIndices[i]);
            }
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
        loadRobot();
        loadFloor();
        //loadMarry();
        loadQuadModel();
    }

    void createColorResources() {
        VkFormat colorFormat = swapChainImageFormat;

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
        colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void createGBufferResources(){
        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, gAlbedoFormatSSAO, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, gAlbedo, gAlbedoMemory);
        gAlbedoView = createImageView(gAlbedo, gAlbedoFormatSSAO, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, gWorldPositionFormatSSAO, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, gViewPosition, gViewPositionMemory);
        gViewPositionView = createImageView(gViewPosition, gWorldPositionFormatSSAO, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, gWorldNormalFormatSSAO, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, gViewNormal, gViewNormalMemory);
        gViewNormalView = createImageView(gViewNormal, gWorldNormalFormatSSAO, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, gDepthFormatSSAO, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, gDepth, gDepthMemory);
        gDepthView = createImageView(gDepth, gDepthFormatSSAO, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void createOcclusionResources(){
        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, gDepthFormatSSAO, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, occlusionImage, occlusionMemory);
        occlusionView = createImageView(occlusionImage, occlusionFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void createDepthResources() {
        VkFormat depthFormat = findDepthFormat();

        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
        depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        //gBufferPasses[0].recordCommandBuffer(commandBuffer, imageIndex);
        //gBufferPasses[1].recordCommandBuffer(commandBuffer, imageIndex);

        //ssaoPass.recordCommandBuffer(commandBuffer, imageIndex);
        blinPhongPasses[0].recordCommandBuffer(commandBuffer, imageIndex);
        blinPhongPasses[1].recordCommandBuffer(commandBuffer, imageIndex);

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
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        gBufferPasses[0].currentFrame = currentFrame;
        //gBufferPasses[1].currentFrame = currentFrame;
        ssaoPass.currentFrame = currentFrame;
        blinPhongPasses[0].currentFrame = currentFrame;
        blinPhongPasses[1].currentFrame = currentFrame;
    }

    void createSSAOJitterNoiseTexture()
    {
        // 1. 生成噪声数据（修改为 glm::vec4）
        std::default_random_engine generator;
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);

        // --- MODIFIED ---
        // 使用 glm::vec4 来匹配 RGBA 格式
        std::vector<glm::vec4> ssaoNoise;
        for (uint32_t i = 0; i < 16; i++) {
            // 将噪声数据放入 x, y 分量，z 保持 0，w (alpha) 设置为 0.0f
            glm::vec4 noise(
                    randomFloats(generator) * 2.0f - 1.0f, // x
                    randomFloats(generator) * 2.0f - 1.0f, // y
                    0.0f,                                  // z
                    0.0f                                   // w (alpha), 未使用
            );
            ssaoNoise.push_back(noise);
        }

        // --- MODIFIED ---
        // 计算新的图像大小
        VkDeviceSize imageSize = 16 * sizeof(glm::vec4);

        // 2. 创建一个暂存缓冲区（Staging Buffer）用于上传数据
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory);

        // 将噪声数据拷贝到暂存缓冲区
        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, ssaoNoise.data(), static_cast<size_t>(imageSize));
        vkUnmapMemory(device, stagingBufferMemory);

        // 3. 创建目标图像（Image）
        const uint32_t texWidth = 4, texHeight = 4;

        // --- MODIFIED ---
        // 使用广泛支持的 VK_FORMAT_R16G16B16A16_SFLOAT 格式
        VkFormat imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = texWidth;
        imageInfo.extent.height = texHeight;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = imageFormat; // 使用新格式
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &imageInfo, nullptr, &noiseTexture.image) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        // 4. 为图像分配内存
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, noiseTexture.image, &memRequirements);
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &noiseTexture.memory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }
        vkBindImageMemory(device, noiseTexture.image, noiseTexture.memory, 0);

        // 5. 使用命令缓冲区完成图像布局转换和数据拷贝
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        // 转换布局：Undefined -> Transfer Destination
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = noiseTexture.image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        // 从暂存缓冲区拷贝到目标图像
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {texWidth, texHeight, 1};
        vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, noiseTexture.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        // 转换布局：Transfer Destination -> Shader Read Only
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        endSingleTimeCommands(commandBuffer);

        // 6. 销毁暂存缓冲区
        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);

        // 7. 创建图像视图（Image View）
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = noiseTexture.image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = imageFormat; // 使用新格式
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &viewInfo, nullptr, &noiseTexture.view) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        // 8. 创建采样器（Sampler）
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_NEAREST;
        samplerInfo.minFilter = VK_FILTER_NEAREST;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &noiseTexture.sampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }

    }

    void DestroyNoiseTexture(){
        vkDestroyImageView(device, noiseTexture.view, nullptr);
        vkFreeMemory(device, noiseTexture.memory, nullptr);
        vkDestroyImage(device, noiseTexture.image, nullptr);
        vkDestroySampler(device, noiseTexture.sampler, nullptr);
    }
};
#endif //VKRENDERINGENGINE_SSAOAPPLICATION_H
