#include "GLFWCallbacks.h"

#include <fmt/format.h>

void FramebufferSizeCallback(GLFWwindow* window,
                             int width,
                             int height)
{
    glViewport(0, 0, width, height);
}

void ErrorCallback(int error_code, const char* description)
{
    fmt::println("[{}]: {}", error_code, description);
}