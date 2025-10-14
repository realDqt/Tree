//
// Created by 22473 on 2025-01-18.
//

#ifndef VKRENDERINGENGINE_UTILS_H
#define VKRENDERINGENGINE_UTILS_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>
#include <random>

#include "../camera.h"
#include "../BaseApplication/BaseApplication.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
std::vector<char> readFile(const std::string& filename);

VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

glm::mat4 aiMat2glmMat(const aiMatrix4x4& rhs);
glm::vec3 aiVec2glmVec(const aiVector3D& rhs);
glm::vec2 aiVec2glmVec(const aiVector2D& rhs);

float lerp(float a, float b, float k);



extern Camera camera;
extern float deltaTime;
extern float lastFrameTime;
extern float lastX;
extern float lastY;
extern bool firstMouse;
extern const uint32_t WIDTH;
extern const uint32_t HEIGHT;
#endif //VKRENDERINGENGINE_UTILS_H
