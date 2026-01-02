#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Envy/core/Defines.h"

ENVY_NAMESPACE_START

class UniformBuffer
{
public:
    UniformBuffer(uint32_t ubo_block_size, uint32_t binding);
    ~UniformBuffer();
    UniformBuffer(const UniformBuffer&) = delete;
    UniformBuffer(UniformBuffer&& other) noexcept;
    UniformBuffer& operator=(const UniformBuffer&) = delete;
    UniformBuffer& operator=(UniformBuffer&& other) noexcept;

    void UploadData(uint32_t offset, uint32_t size, const void* data) const;

private:
    GLuint m_uboID = GL_NONE;
    uint32_t m_uboBlockSize = 0;
};

ENVY_NAMESPACE_END