cmake_minimum_required(VERSION 3.27)
project(vkRenderingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置默认构建类型为 Release
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(TREE_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party" CACHE PATH
        "Directory containing Tree's third-party dependencies")

set(GLFW_PATH "${TREE_THIRD_PARTY_DIR}/glfw")
set(VULKAN_PATH "${TREE_THIRD_PARTY_DIR}/VulkanSDK")
set(STB_IMAGE_PATH "${TREE_THIRD_PARTY_DIR}/stb")
set(TINY_OBJ_LOADER_PATH "${TREE_THIRD_PARTY_DIR}/tinyobjloader")
set(GLM_PATH "${TREE_THIRD_PARTY_DIR}/glm")
set(ASSIMP_PATH "${TREE_THIRD_PARTY_DIR}/assimp")

set(TREE_REQUIRED_DEPENDENCY_FILES
        "${GLFW_PATH}/CMakeLists.txt"
        "${VULKAN_PATH}/Include/vulkan/vulkan.h"
        "${VULKAN_PATH}/Lib/vulkan-1.lib"
        "${VULKAN_PATH}/Bin/glslangValidator.exe"
        "${VULKAN_PATH}/Bin/VkLayer_khronos_validation.json"
        "${VULKAN_PATH}/Bin/VkLayer_khronos_validation.dll"
        "${STB_IMAGE_PATH}/stb_image.h"
        "${STB_IMAGE_PATH}/stb_image_write.h"
        "${TINY_OBJ_LOADER_PATH}/tiny_obj_loader.h"
        "${GLM_PATH}/glm/glm.hpp"
        "${ASSIMP_PATH}/CMakeLists.txt"
)

foreach(dependency_file IN LISTS TREE_REQUIRED_DEPENDENCY_FILES)
    if(NOT EXISTS "${dependency_file}")
        message(FATAL_ERROR
                "Missing dependency file: ${dependency_file}\n"
                "Run scripts/setup_dependencies.ps1 before configuring the project.")
    endif()
endforeach()

# Build the source dependencies as part of this project. Keeping these as CMake
# targets avoids hard-coding compiler-specific library names and Debug/Release
# output directories.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static third-party libraries" FORCE)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("${GLFW_PATH}" "${CMAKE_BINARY_DIR}/third_party/glfw" EXCLUDE_FROM_ALL)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)
if(POLICY CMP0175)
    # Assimp 5.4.3 relies on the pre-CMake-3.31 add_custom_command behavior.
    set(CMAKE_POLICY_DEFAULT_CMP0175 OLD)
endif()
add_subdirectory("${ASSIMP_PATH}" "${CMAKE_BINARY_DIR}/third_party/assimp" EXCLUDE_FROM_ALL)
unset(CMAKE_POLICY_DEFAULT_CMP0175)

add_library(TreeVulkan INTERFACE)
add_library(Vulkan::Vulkan ALIAS TreeVulkan)
target_include_directories(TreeVulkan SYSTEM INTERFACE "${VULKAN_PATH}/Include")
target_link_libraries(TreeVulkan INTERFACE "${VULKAN_PATH}/Lib/vulkan-1.lib")

set(TREE_THIRD_PARTY_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${STB_IMAGE_PATH}
        ${TINY_OBJ_LOADER_PATH}
        ${GLM_PATH}
)

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
target_compile_definitions(TreeCommon PUBLIC NOMINMAX)
target_link_libraries(TreeCommon
        PUBLIC
        glfw
        Vulkan::Vulkan
        assimp::assimp
)

function(add_tree_application target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} PRIVATE TreeCommon)
    set_target_properties(${target} PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            VS_DEBUGGER_ENVIRONMENT "VK_ADD_LAYER_PATH=${VULKAN_PATH}/Bin"
    )
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
        SSRApplication/FXAAPass.h
        SSRApplication/FXAAPass.cpp
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
