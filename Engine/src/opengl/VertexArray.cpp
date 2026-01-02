#include "Envy/opengl/VertexArray.h"

ENVY_NAMESPACE_START

// TODO: Add bits to specify attributes
VertexArray::VertexArray(const Vertex* vertices, uint32_t n_vertices,
                         const GLuint* indices, uint32_t n_indices)
{
    const GLuint VBO_BIND_INDEX_PER_VERTEX = 0;
    const GLuint VBO_BIND_INDEX_PER_INSTANCE = 1;

    glCreateBuffers(1, &m_vboID);
    glNamedBufferStorage(m_vboID,
                         n_vertices * sizeof(Vertex),
                         vertices,
                         GL_DYNAMIC_STORAGE_BIT);
                         
    glCreateBuffers(1, &m_eboID);
    glNamedBufferStorage(m_eboID,
                         n_indices * sizeof(GLuint),
                         indices,
                         GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &m_vaoID);
    glVertexArrayVertexBuffer(m_vaoID, VBO_BIND_INDEX_PER_VERTEX, m_vboID, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_vaoID, m_eboID);

    glEnableVertexArrayAttrib(m_vaoID, 0);
    glEnableVertexArrayAttrib(m_vaoID, 1);
    glEnableVertexArrayAttrib(m_vaoID, 2);
    // glEnableVertexArrayAttrib(m_vaoID, 3);

    glVertexArrayAttribFormat(m_vaoID, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(m_vaoID, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexArrayAttribFormat(m_vaoID, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
    // glVertexArrayAttribFormat(m_vaoID, 0, 1, GL_UNSIGNED_INT, GL_FALSE, 0);

    glVertexArrayAttribBinding(m_vaoID, 0, VBO_BIND_INDEX_PER_VERTEX);
    glVertexArrayAttribBinding(m_vaoID, 1, VBO_BIND_INDEX_PER_VERTEX);
    glVertexArrayAttribBinding(m_vaoID, 2, VBO_BIND_INDEX_PER_VERTEX);
    // glVertexArrayAttribBinding(m_vaoID, 3, VBO_BIND_INDEX_PER_INSTANCE);

    // glVertexArrayBindingDivisor(m_vaoID, 3, 1);
    // glVertexAttribDivisor(3, 1);
    // glVertexBindingDivisor(3, 1);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_vaoID);
    glDeleteBuffers(1, &m_vboID);
    glDeleteBuffers(1, &m_eboID);

    m_vaoID = GL_NONE;
    m_vboID = GL_NONE;
    m_eboID = GL_NONE;
}

VertexArray::VertexArray(VertexArray&& other) noexcept
{
    m_vboID = other.m_vboID;
    m_eboID = other.m_eboID;
    m_vaoID = other.m_vaoID;

    other.m_vboID = GL_NONE;
    other.m_eboID = GL_NONE;
    other.m_vaoID = GL_NONE;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    m_vboID = other.m_vboID;
    m_eboID = other.m_eboID;
    m_vaoID = other.m_vaoID;

    other.m_vboID = GL_NONE;
    other.m_eboID = GL_NONE;
    other.m_vaoID = GL_NONE;

    return *this;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_vaoID);
}

ENVY_NAMESPACE_END