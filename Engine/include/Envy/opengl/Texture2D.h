#pragma once

#include <glad/glad.h>

#include "Envy/Envy.h"

ENVY_NAMESPACE_START

class Texture2D
{
public:
    Texture2D(int width,
              int height,
              TextureFormat format,
              const Sampler2DConfig& sampler2DConfig = Sampler2DConfig());
    ~Texture2D();
    Texture2D(const Texture2D&) = delete;
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D& operator=(Texture2D&& other) noexcept;

    void Bind(GLuint unit) const;
    void MakeImageWriteAccess(GLuint unit) const;
    void MakeImageReadAccess(GLuint unit) const;
    void UploadImage(uint8_t* data, int width, int height) const;

private:
    GLuint m_textureID = GL_NONE;
    TextureFormat m_format = TextureFormat::NONE;
};

ENVY_NAMESPACE_END