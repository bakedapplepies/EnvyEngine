#include "Envy/opengl/Framebuffer.h"

#include "Envy/core/Assert.h"

ENVY_NAMESPACE_START

Framebuffer::Framebuffer(int width, int height)
{
    glCreateFramebuffers(1, &m_fboID);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
    glTextureParameteri(m_colorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_colorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_colorAttachment, 1, GL_RGBA8, width, height);
    glNamedFramebufferTexture(m_fboID, GL_COLOR_ATTACHMENT0, m_colorAttachment, 0);

    glCreateRenderbuffers(1, &m_rbo);
    glNamedRenderbufferStorage(m_rbo, GL_DEPTH24_STENCIL8, width, height);
    glNamedFramebufferRenderbuffer(m_fboID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    bool result = glCheckNamedFramebufferStatus(m_fboID, GL_FRAMEBUFFER);
    ENVY_ASSERT("Framebuffer creation error.", result);
}

Framebuffer::~Framebuffer()
{
    glDeleteTextures(1, &m_colorAttachment);
    glDeleteRenderbuffers(1, &m_rbo);
    glDeleteFramebuffers(1, &m_fboID);

    m_fboID = GL_NONE;
    m_colorAttachment = GL_NONE;
    m_rbo = GL_NONE;
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
{
    m_fboID = other.m_fboID;
    m_colorAttachment = other.m_colorAttachment;
    m_rbo = other.m_rbo;

    other.m_fboID = GL_NONE;
    other.m_colorAttachment = GL_NONE;
    other.m_rbo = GL_NONE;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
    m_fboID = other.m_fboID;
    m_colorAttachment = other.m_colorAttachment;
    m_rbo = other.m_rbo;

    other.m_fboID = GL_NONE;
    other.m_colorAttachment = GL_NONE;
    other.m_rbo = GL_NONE;

    return *this;
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
}

void Framebuffer::BindColorAttachment(GLuint unit) const
{
    glBindTextureUnit(unit, m_colorAttachment);
}

void Framebuffer::MakeColorAttachmentImageWriteAccess(GLuint unit) const
{
    glBindImageTexture(unit,
                       m_colorAttachment,
                       0, GL_FALSE,
                       0,
                       GL_WRITE_ONLY,
                       GL_RGBA8);
}

void Framebuffer::MakeColorAttachmentImageReadAccess(GLuint unit) const
{
    glBindImageTexture(unit,
                       m_colorAttachment,
                       0, GL_FALSE,
                       0,
                       GL_READ_ONLY,
                       GL_RGBA8);
}

ENVY_NAMESPACE_END