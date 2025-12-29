#pragma once

#include <glad/glad.h>

#include "Envy/core/Defines.h"

ENVY_NAMESPACE_START

extern void GLAPIENTRY DebugMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    [[maybe_unused]] GLsizei length,
    const GLchar* message,
    [[maybe_unused]] const void* userParam
);

ENVY_NAMESPACE_END