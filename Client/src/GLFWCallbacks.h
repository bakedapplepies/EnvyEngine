#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern void FramebufferSizeCallback(GLFWwindow* window,
                                    int width,
                                    int height);
extern void ErrorCallback(int error_code, const char* description);