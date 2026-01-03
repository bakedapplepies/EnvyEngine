#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Envy/core/Defines.h"

ENVY_NAMESPACE_START

const GLuint VBO_BIND_INDEX_PER_VERTEX = 0;
const GLuint VBO_BIND_INDEX_PER_INSTANCE = 1;

const GLuint VAO_ATTRIB_INDEX_POSITION = 0;
const GLuint VAO_ATTRIB_INDEX_NORMAL = 1;
const GLuint VAO_ATTRIB_INDEX_UV = 2;
const GLuint VAO_ATTRIB_INDEX_INSTANCE_0 = 3;

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
    int vertexOffset;
};

struct IndirectCommand
{
    const Envy::VAOChunk* vaoChunk = nullptr;
    uint32_t instanceCount = 1;
    uint32_t baseInstance = 0;
};

struct Sampler2DConfig
{
    GLenum wrapX = GL_CLAMP_TO_EDGE;
    GLenum wrapY = GL_CLAMP_TO_EDGE;
    GLenum closeFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum farFilter = GL_NEAREST;
};

struct Sampler3DConfig
{
    GLenum wrapX = GL_CLAMP_TO_EDGE;
    GLenum wrapY = GL_CLAMP_TO_EDGE;
    GLenum wrapZ = GL_CLAMP_TO_EDGE;
    GLenum closeFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum farFilter = GL_NEAREST;
};

enum class FaceOrder : GLenum
{
    CLOCKWISE = GL_CW,
    COUNTER_CLOCKWISE = GL_CCW,
    NONE  
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