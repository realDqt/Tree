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

#include "camera.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

extern Camera camera;
extern float deltaTime;
extern float lastFrameTime;
extern float lastX;
extern float lastY;
extern bool firstMouse;
extern const uint32_t WIDTH;
extern const uint32_t HEIGHT;
#endif //VKRENDERINGENGINE_UTILS_H
