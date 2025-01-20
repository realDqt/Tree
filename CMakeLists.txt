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
        HelloBoxApplication/HelloBoxApplication.h
        Light.h
)

target_link_libraries(vkRenderingEngine glfw3 vulkan-1)

