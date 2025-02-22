cmake_minimum_required(VERSION 3.27)
project(vkRenderingEngine)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_PATH E:/glfw/glfw-3.4.bin.WIN64)
set(VULKAN_PATH E:/Vulkan/VulkanSDK)
set(STB_IMAGE_PATH E:/ImageProcess)
set(TINY_OBJ_LOADER_PATH E:/ObjLoader)
set(GLAD_PATH E:/glad)

include_directories(${GLFW_PATH}/include)
include_directories(${VULKAN_PATH}/include)
include_directories(${STB_IMAGE_PATH})
include_directories(${TINY_OBJ_LOADER_PATH})
include_directories(${GLAD_PATH}/include)


link_directories(${GLFW_PATH}/lib-mingw-w64)
link_directories(${VULKAN_PATH}/Lib)


add_executable(vkRenderingEngine
        main.cpp
        Vertex.h
        camera.h
        utils.h
        utils.cpp
        PCSSApplication/PCSSApplication.h
        Light.h
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
        VulkanInitializers.hpp
        VulkanTools.h
        VulkanTools.cpp
        IBLApplication/SpherePass.h
        IBLApplication/SpherePass.cpp
        IBLApplication/IrradianceMapPass.h
        IBLApplication/IrradianceMapPass.cpp
        IBLApplication/PrefilterMapPass.h
        IBLApplication/PrefilterMapPass.cpp
)

target_link_libraries(vkRenderingEngine glfw3 vulkan-1)

