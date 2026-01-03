#include "Envy/opengl/VertexArray.h"

ENVY_NAMESPACE_START

// TODO: Add bits to specify attributes
VertexArray::VertexArray(const Vertex* vertices, uint32_t n_vertices,
                         const GLuint* indices, uint32_t n_indices)
{
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

    glEnableVertexArrayAttrib(m_vaoID, VAO_ATTRIB_INDEX_POSITION);
    glEnableVertexArrayAttrib(m_vaoID, VAO_ATTRIB_INDEX_NORMAL);
    glEnableVertexArrayAttrib(m_vaoID, VAO_ATTRIB_INDEX_UV);

    glVertexArrayAttribFormat(m_vaoID, VAO_ATTRIB_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(m_vaoID, VAO_ATTRIB_INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexArrayAttribFormat(m_vaoID, VAO_ATTRIB_INDEX_UV, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

    glVertexArrayAttribBinding(m_vaoID, VAO_ATTRIB_INDEX_POSITION, VBO_BIND_INDEX_PER_VERTEX);
    glVertexArrayAttribBinding(m_vaoID, VAO_ATTRIB_INDEX_NORMAL, VBO_BIND_INDEX_PER_VERTEX);
    glVertexArrayAttribBinding(m_vaoID, VAO_ATTRIB_INDEX_UV, VBO_BIND_INDEX_PER_VERTEX);

    glCreateBuffers(1, &m_instanceBufferID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_vaoID);
    glDeleteBuffers(1, &m_vboID);
    glDeleteBuffers(1, &m_eboID);
    glDeleteBuffers(1, &m_instanceBufferID);

    m_vaoID = GL_NONE;
    m_vboID = GL_NONE;
    m_eboID = GL_NONE;
    m_instanceBufferID = GL_NONE;
}

VertexArray::VertexArray(VertexArray&& other) noexcept
{
    m_vboID = other.m_vboID;
    m_eboID = other.m_eboID;
    m_vaoID = other.m_vaoID;
    m_instanceBufferID = other.m_instanceBufferID;

    other.m_vboID = GL_NONE;
    other.m_eboID = GL_NONE;
    other.m_vaoID = GL_NONE;
    other.m_instanceBufferID = GL_NONE;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    m_vboID = other.m_vboID;
    m_eboID = other.m_eboID;
    m_vaoID = other.m_vaoID;
    m_instanceBufferID = other.m_instanceBufferID;

    other.m_vboID = GL_NONE;
    other.m_eboID = GL_NONE;
    other.m_vaoID = GL_NONE;
    other.m_instanceBufferID = GL_NONE;

    return *this;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_vaoID);
}

void VertexArray::AddInstanceBuffer(uint32_t components,
                                    uint32_t stride,
                                    uint32_t size,
                                    const void* data) const
{
    glNamedBufferStorage(m_instanceBufferID, stride * size, data, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(m_vaoID, VBO_BIND_INDEX_PER_INSTANCE, m_instanceBufferID, 0, stride);
    glEnableVertexArrayAttrib(m_vaoID, VAO_ATTRIB_INDEX_INSTANCE_0);
    glVertexArrayAttribFormat(m_vaoID, VAO_ATTRIB_INDEX_INSTANCE_0, components, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_vaoID, VAO_ATTRIB_INDEX_INSTANCE_0, VBO_BIND_INDEX_PER_INSTANCE);
    glVertexArrayBindingDivisor(m_vaoID, VBO_BIND_INDEX_PER_INSTANCE, 1);
}

ENVY_NAMESPACE_END