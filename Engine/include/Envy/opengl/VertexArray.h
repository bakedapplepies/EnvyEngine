#pragma once

#include <glad/glad.h>

#include "Envy/Envy.h"

ENVY_NAMESPACE_START

class VertexArray
{
public:
    VertexArray(const Vertex* vertices, uint32_t n_vertices,
                const GLuint* indices, uint32_t n_indices);
    ~VertexArray();
    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void Bind() const;

private:
    GLuint m_vboID = GL_NONE;
    GLuint m_eboID = GL_NONE;
    GLuint m_vaoID = GL_NONE;
};

ENVY_NAMESPACE_END