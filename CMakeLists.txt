cmake_minimum_required(VERSION 3.27)
project(vkRenderingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置默认构建类型为 Release
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(GLFW_PATH E:/glfw/glfw-3.4.bin.WIN64)
set(VULKAN_PATH E:/Vulkan/VulkanSDK)
set(STB_IMAGE_PATH E:/ImageProcess)
set(TINY_OBJ_LOADER_PATH E:/ObjLoader)
set(GLAD_PATH E:/glad)
set(TINY_GLTF_LOADER_PATH E:/GLTFLoader)
set(ASSIMP_PATH E:/assimp/assimp-master)

find_path(STB_IMAGE_WRITE_INCLUDE_DIR
        NAMES stb_image_write.h
        HINTS
        ${STB_IMAGE_PATH}
        E:/Proj/SlnProj/nvpro_core2/third_party/stb
)

set(TREE_THIRD_PARTY_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${GLFW_PATH}/include
        ${VULKAN_PATH}/include
        ${STB_IMAGE_PATH}
        ${TINY_OBJ_LOADER_PATH}
        ${TINY_GLTF_LOADER_PATH}
        ${GLAD_PATH}/include
        ${ASSIMP_PATH}/include
)

set(TREE_THIRD_PARTY_LINK_DIRS
        ${GLFW_PATH}/lib-vc2022
        ${VULKAN_PATH}/Lib
        ${ASSIMP_PATH}/build/lib/Debug
        ${ASSIMP_PATH}/build/lib/Release
)

set(ASSIMP_DEBUG_DLL ${ASSIMP_PATH}/build/bin/Debug/assimp-vc143-mtd.dll)
set(ASSIMP_RELEASE_DLL ${ASSIMP_PATH}/build/bin/Release/assimp-vc143-mt.dll)

add_library(TreeCommon STATIC
        BaseApplication/BaseApplication.cpp
        BaseApplication/BaseApplication.h
        camera.h
        EngineCore/Light.h
        EngineCore/utils.cpp
        EngineCore/utils.h
        EngineCore/Vertex.h
        EngineCore/VulkanInitializers.hpp
        EngineCore/VulkanTools.cpp
        EngineCore/VulkanTools.h
        EngineCore/RenderPass/RenderPass.cpp
        EngineCore/RenderPass/RenderPass.h
        EngineCore/RenderPass/PresentPass.cpp
        EngineCore/RenderPass/PresentPass.h
)

target_include_directories(TreeCommon PUBLIC ${TREE_THIRD_PARTY_INCLUDE_DIRS})
target_link_directories(TreeCommon PUBLIC ${TREE_THIRD_PARTY_LINK_DIRS})
target_compile_definitions(TreeCommon PUBLIC NOMINMAX)
target_link_libraries(TreeCommon
        PUBLIC
        glfw3
        vulkan-1
        debug assimp-vc143-mtd
        optimized assimp-vc143-mt
)

function(add_tree_application target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} PRIVATE TreeCommon)
    set_target_properties(${target} PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    if(EXISTS ${ASSIMP_DEBUG_DLL})
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${ASSIMP_DEBUG_DLL}
                $<TARGET_FILE_DIR:${target}>
                COMMAND_EXPAND_LISTS
        )
    endif()

    if(EXISTS ${ASSIMP_RELEASE_DLL})
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${ASSIMP_RELEASE_DLL}
                $<TARGET_FILE_DIR:${target}>
                COMMAND_EXPAND_LISTS
        )
    endif()
endfunction()

add_tree_application(HelloBoxApplication
        targets/HelloBoxApplicationMain.cpp
        HelloBoxApplication/HelloBoxApplication.h
)

add_tree_application(PCSSApplication
        targets/PCSSApplicationMain.cpp
        PCSSApplication/PCSSApplication.h
        PCSSApplication/ShadowmapPass.h
        PCSSApplication/ShadowmapPass.cpp
        PCSSApplication/PCSSutils.h
        PCSSApplication/PCSSutils.cpp
        PCSSApplication/BlinPhongPass.h
        PCSSApplication/BlinPhongPass.cpp
        PCSSApplication/LightVisualizationPass.h
        PCSSApplication/LightVisualization.cpp
)

add_tree_application(IBLApplication
        targets/IBLApplicationMain.cpp
        IBLApplication/IBLApplication.h
        IBLApplication/HDRVisualizationPass.h
        IBLApplication/HDRVisualizationPass.cpp
        IBLApplication/IBLutils.h
        IBLApplication/IBLutils.cpp
        IBLApplication/CubemapPass.h
        IBLApplication/CubemapPass.cpp
        IBLApplication/SkyboxPass.h
        IBLApplication/SkyboxPass.cpp
        IBLApplication/SpherePass.h
        IBLApplication/SpherePass.cpp
        IBLApplication/IrradianceMapPass.h
        IBLApplication/IrradianceMapPass.cpp
        IBLApplication/PrefilterMapPass.h
        IBLApplication/PrefilterMapPass.cpp
        IBLApplication/BrdfLUTPass.h
        IBLApplication/BrdfLUTPass.cpp
)

add_tree_application(SSRApplication
        targets/SSRApplicationMain.cpp
        SSRApplication/SSRApplication.h
        SSRApplication/SSRutils.h
        SSRApplication/SSRutils.cpp
        SSRApplication/BlinPhongPassSSR.h
        SSRApplication/BlinPhongPassSSR.cpp
        SSRApplication/SSRShadowmapPass.h
        SSRApplication/SSRShadowmapPass.cpp
        SSRApplication/GBufferPass.h
        SSRApplication/GBufferPass.cpp
        SSRApplication/GenerateHizPass.h
        SSRApplication/GenerateHizPass.cpp
        SSRApplication/SSRPass.h
        SSRApplication/SSRPass.cpp
)

add_tree_application(FFTBloomApplication
        targets/FFTBloomApplicationMain.cpp
        FFTBloomApplication/FFTBloomApplication.h
        FFTBloomApplication/FFTBloomUtils.h
        FFTBloomApplication/FFTBloomUtils.cpp
        FFTBloomApplication/FBSkyboxPass.h
        FFTBloomApplication/FBSkyboxPass.cpp
        FFTBloomApplication/FBCubemapPass.h
        FFTBloomApplication/FBCubemapPass.cpp
        FFTBloomApplication/NativeBloomPass.h
        FFTBloomApplication/NativeBloomPass.cpp
)

if(STB_IMAGE_WRITE_INCLUDE_DIR)
    target_include_directories(FFTBloomApplication PRIVATE ${STB_IMAGE_WRITE_INCLUDE_DIR})
endif()

add_tree_application(SSAOApplication
        targets/SSAOApplicationMain.cpp
        SSAOApplication/SSAOApplication.h
        SSAOApplication/SSAOGBufferPass.h
        SSAOApplication/SSAOGBufferPass.cpp
        SSAOApplication/SSAOutils.h
        SSAOApplication/SSAOutils.cpp
        SSAOApplication/SSAOPass.h
        SSAOApplication/SSAOPass.cpp
        SSAOApplication/DeferredLightPass.h
        SSAOApplication/DeferredLightPass.cpp
        SSAOApplication/BlurPass.h
        SSAOApplication/BlurPass.cpp
)
