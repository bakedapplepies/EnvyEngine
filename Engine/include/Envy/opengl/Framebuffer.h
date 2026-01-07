#pragma once

#include <glad/glad.h>

#include "Envy/core/Defines.h"

ENVY_NAMESPACE_START

class Framebuffer
{
public:
    Framebuffer(int width, int height);
    ~Framebuffer();
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(const Framebuffer&) = delete;
    Framebuffer& operator=(Framebuffer&& other) noexcept;

    void Bind() const;
    void BindColorAttachment(GLuint unit) const;
    void MakeColorAttachmentImageWriteAccess(GLuint unit) const;
    void MakeColorAttachmentImageReadAccess(GLuint unit) const;

private:
    GLuint m_fboID = GL_NONE;
    GLuint m_colorAttachment = GL_NONE;
    GLuint m_rbo = GL_NONE;
};

ENVY_NAMESPACE_END