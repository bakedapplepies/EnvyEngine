#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Envy/core/Defines.h"

ENVY_NAMESPACE_START

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct VAOChunk
{
    uint32_t elementsOffset;
    uint32_t elementsCount;
    uint32_t vertex_offset;
};

struct Sampler2DConfig
{
    GLenum wrapX = GL_CLAMP_TO_EDGE;
    GLenum wrapY = GL_CLAMP_TO_EDGE;
    GLenum closeFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum farFilter = GL_NEAREST;
};

enum class ShaderType : GLenum
{
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    COMPUTE = GL_COMPUTE_SHADER,
    NONE
};

enum class TextureFormat : GLenum
{
    RGB8 = GL_RGB8,
    RGBA8 = GL_RGBA8,
    RGBA32F = GL_RGBA32F,
    NONE
};

ENVY_NAMESPACE_END