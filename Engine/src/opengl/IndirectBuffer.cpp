#include "Envy/opengl/IndirectBuffer.h"

ENVY_NAMESPACE_START

IndirectBuffer::IndirectBuffer(uint32_t command_count, const IndirectCommand* commands)
{
    glCreateBuffers(1, &m_bufferID);
    glNamedBufferStorage(m_bufferID,
                         sizeof(IndirectCommand) * command_count,
                         commands,
                         GL_DYNAMIC_STORAGE_BIT);

    m_commandCount = command_count;
};

IndirectBuffer::~IndirectBuffer()
{
    glDeleteBuffers(1, &m_bufferID);

    m_bufferID = GL_NONE;
    m_commandCount = 0;
}

IndirectBuffer::IndirectBuffer(IndirectBuffer&& other) noexcept
{
    m_bufferID = other.m_bufferID;
    m_commandCount = other.m_commandCount;

    other.m_bufferID = GL_NONE;
    other.m_commandCount = 0;
}

IndirectBuffer& IndirectBuffer::operator=(IndirectBuffer&& other) noexcept
{
    m_bufferID = other.m_bufferID;
    m_commandCount = other.m_commandCount;

    other.m_bufferID = GL_NONE;
    other.m_commandCount = 0;

    return *this;
}

void IndirectBuffer::Bind() const
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_bufferID);
}

uint32_t IndirectBuffer::GetCommandCount() const
{
    return m_commandCount;
}

ENVY_NAMESPACE_END