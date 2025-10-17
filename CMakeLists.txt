cmake_minimum_required(VERSION 3.27)
project(vkRenderingEngine)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_PATH E:/glfw/glfw-3.4.bin.WIN64)
set(VULKAN_PATH E:/Vulkan/VulkanSDK)
set(STB_IMAGE_PATH E:/ImageProcess)
set(TINY_OBJ_LOADER_PATH E:/ObjLoader)
set(GLAD_PATH E:/glad)
set(TINY_GLTF_LOADER_PATH E:/GLTFLoader)
set(ASSIMP_PATH E:/assimp/assimp-master)

include_directories(${GLFW_PATH}/include)
include_directories(${VULKAN_PATH}/include)
include_directories(${STB_IMAGE_PATH})
include_directories(${TINY_OBJ_LOADER_PATH})
include_directories(${TINY_GLTF_LOADER_PATH})
include_directories(${GLAD_PATH}/include)
include_directories(${ASSIMP_PATH}/include)


link_directories(${GLFW_PATH}/lib-vc2022)
link_directories(${VULKAN_PATH}/Lib)
link_directories(${ASSIMP_PATH}/build/lib/Debug)

add_definitions(-DNOMINMAX)



add_executable(vkRenderingEngine
        main.cpp
        EngineCore/Vertex.h
        camera.h
        EngineCore/utils.h
        EngineCore/utils.cpp
        PCSSApplication/PCSSApplication.h
        EngineCore/Light.h
        BaseApplication/BaseApplication.h
        BaseApplication/BaseApplication.cpp
        HelloBoxApplication/HelloBoxApplication.h
        PCSSApplication/ShadowmapPass.h
        PCSSApplication/ShadowmapPass.cpp
        PCSSApplication/PCSSutils.h
        PCSSApplication/PCSSutils.cpp
        PCSSApplication/BlinPhongPass.h
        PCSSApplication/BlinPhongPass.cpp
        PCSSApplication/LightVisualizationPass.h
        PCSSApplication/LightVisualization.cpp
        IBLApplication/IBLApplication.h
        IBLApplication/HDRVisualizationPass.h
        IBLApplication/IBLutils.h
        IBLApplication/IBLutils.cpp
        IBLApplication/HDRVisualizationPass.cpp
        IBLApplication/CubemapPass.h
        IBLApplication/CubemapPass.cpp
        IBLApplication/SkyboxPass.h
        IBLApplication/SkyboxPass.cpp
        EngineCore/VulkanInitializers.hpp
        EngineCore/VulkanTools.h
        EngineCore/VulkanTools.cpp
        IBLApplication/SpherePass.h
        IBLApplication/SpherePass.cpp
        IBLApplication/IrradianceMapPass.h
        IBLApplication/IrradianceMapPass.cpp
        IBLApplication/PrefilterMapPass.h
        IBLApplication/PrefilterMapPass.cpp
        IBLApplication/BrdfLUTPass.h
        IBLApplication/BrdfLUTPass.cpp
        SSRApplication/SSRApplication.h
        SSRApplication/SSRutils.h
        SSRApplication/SSRutils.cpp
        SSRApplication/BlinPhongPassSSR.h
        SSRApplication/BlinPhongPassSSR.cpp
        SSRApplication/SSRShadowmapPass.h
        SSRApplication/SSRShadowmapPass.cpp
        SSRApplication/GBufferPass.h
        SSRApplication/GBufferPass.cpp
        FFTBloomApplication/FFTBloomUtils.h
        FFTBloomApplication/FBSkyboxPass.h
        FFTBloomApplication/FBSkyboxPass.cpp
        FFTBloomApplication/FBCubemapPass.h
        FFTBloomApplication/FBCubemapPass.h
        FFTBloomApplication/FBCubemapPass.h
        FFTBloomApplication/FBCubemapPass.cpp
        FFTBloomApplication/FFTBloomApplication.h
        FFTBloomApplication/FFTBloomUtils.cpp
        FFTBloomApplication/NativeBloomPass.h
        FFTBloomApplication/NativeBloomPass.h
        FFTBloomApplication/NativeBloomPass.cpp
        FFTBloomApplication/NativeBloomPass.cpp
        SSRApplication/SSRPass.h
        SSRApplication/SSRPass.cpp
        EngineCore/RenderPass/RenderPass.h
        EngineCore/RenderPass/RenderPass.cpp
        EngineCore/RenderPass/PresentPass.h
        EngineCore/RenderPass/PresentPass.cpp
        SSAOApplication/SSAOGBufferPass.cpp
        SSAOApplication/SSAOGBufferPass.h
        SSAOApplication/SSAOutils.h
        SSAOApplication/SSAOutils.cpp
        SSAOApplication/SSAOApplication.h
        SSAOApplication/SSAOPass.h
        SSAOApplication/SSAOPass.cpp
        SSAOApplication/DeferredLightPass.h
        SSAOApplication/DeferredLightPass.cpp
        SSAOApplication/BlurPass.h
        SSAOApplication/BlurPass.cpp
)

target_link_libraries(vkRenderingEngine glfw3 vulkan-1 assimp-vc143-mtd)

