#include "Envy/opengl/Texture2D.h"

ENVY_NAMESPACE_START

Texture2D::Texture2D(int width,
                     int height,
                     TextureFormat format,
                     const Sampler2DConfig& sampler2DConfig)
{
    GLenum glFormat = static_cast<GLenum>(format);
    m_format = format;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, sampler2DConfig.wrapX);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, sampler2DConfig.wrapY);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, sampler2DConfig.closeFilter);
    glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, sampler2DConfig.farFilter);

    glTextureStorage2D(m_textureID, 1, glFormat, width, height);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_textureID);

    m_textureID = GL_NONE;
    m_format = TextureFormat::NONE;
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
    m_textureID = other.m_textureID;
    m_format = other.m_format;

    other.m_textureID = 0;
    other.m_format = TextureFormat::NONE;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    m_textureID = other.m_textureID;
    m_format = other.m_format;

    other.m_textureID = 0;
    other.m_format = TextureFormat::NONE;

    return *this;
}

void Texture2D::Bind(GLuint slot) const
{
    glBindTextureUnit(slot, m_textureID);
}

void Texture2D::MakeWriteAccess(GLuint unit) const
{
    glBindImageTexture(unit,
                       m_textureID,
                       0, GL_FALSE,
                       0,
                       GL_WRITE_ONLY,
                       static_cast<GLenum>(m_format));
}

void Texture2D::UploadImage(uint8_t* data, int width, int height) const
{
    static const int LEVEL = 0;
    static const int X_OFFSET = 0;
    static const int Y_OFFSET = 0;
    glTextureSubImage2D(m_textureID,
                        LEVEL,
                        X_OFFSET, Y_OFFSET,
                        width, height,
                        GL_RGBA, GL_UNSIGNED_BYTE, data);
}

ENVY_NAMESPACE_END