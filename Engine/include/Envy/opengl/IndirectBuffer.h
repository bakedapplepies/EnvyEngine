#pragma once

#include <glad/glad.h>

#include "Envy/core/Defines.h"
#include "Envy/Envy.h"

ENVY_NAMESPACE_START

class IndirectBuffer
{
public:
    IndirectBuffer(uint32_t command_count, const IndirectCommand* commands);
    ~IndirectBuffer();
    IndirectBuffer(const IndirectBuffer&) = delete;
    IndirectBuffer(IndirectBuffer&& other) noexcept;
    IndirectBuffer& operator=(const IndirectBuffer&) = delete;
    IndirectBuffer& operator=(IndirectBuffer&& other) noexcept;

    void Bind() const;
    uint32_t GetCommandCount() const;

private:
    GLuint m_bufferID = GL_NONE;
    uint32_t m_commandCount = 0;
};

ENVY_NAMESPACE_END