//
// Created by 22473 on 2025-01-18.
//

#ifndef VKRENDERINGENGINE_UTILS_H
#define VKRENDERINGENGINE_UTILS_H
#include <GLFW/glfw3.h>
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
