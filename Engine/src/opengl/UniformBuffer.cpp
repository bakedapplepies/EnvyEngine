#include "Envy/opengl/UniformBuffer.h"

ENVY_NAMESPACE_START

UniformBuffer::UniformBuffer(uint32_t ubo_block_size, uint32_t binding)
{
    glCreateBuffers(1, &m_uboID);
    glNamedBufferStorage(m_uboID, ubo_block_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_uboID);

    m_uboBlockSize = ubo_block_size;
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_uboID);

    m_uboID = GL_NONE;
    m_uboBlockSize = 0;
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
{
    m_uboID = other.m_uboID;
    m_uboBlockSize = other.m_uboBlockSize;

    other.m_uboID = GL_NONE;
    other.m_uboBlockSize = 0;
}

UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
{
    m_uboID = other.m_uboID;
    m_uboBlockSize = other.m_uboBlockSize;

    other.m_uboID = GL_NONE;
    other.m_uboBlockSize = 0;

    return *this;
}

void UniformBuffer::UploadData(uint32_t offset, uint32_t size, const void* data) const
{
    glNamedBufferSubData(m_uboID, offset, size, data);
}

ENVY_NAMESPACE_END