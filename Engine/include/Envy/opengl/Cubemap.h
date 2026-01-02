#pragma once

#include <memory>

#include "Envy/core/Defines.h"
#include "Envy/Envy.h"
#include "Envy/opengl/Pipeline.h"

ENVY_NAMESPACE_START

class Cubemap
{
public:
    Cubemap(int width,
            int height,
            TextureFormat format,
            const std::array<uint8_t*, 6>& data,
            const Sampler3DConfig& sampler3DConfig = Sampler3DConfig());
    ~Cubemap();
    Cubemap(const Cubemap&) = delete;
    Cubemap(Cubemap&& other) noexcept;
    Cubemap& operator=(const Cubemap&) = delete;
    Cubemap& operator=(Cubemap&& other) noexcept;

    void Draw() const;

private:
    GLuint m_cubemapID = GL_NONE;
    GLuint m_vaoID = GL_NONE;
    GLuint m_vboID = GL_NONE;
    GLuint m_eboID = GL_NONE;

    TextureFormat m_format = TextureFormat::NONE;
    ShaderProgram m_vertexShader;
    ShaderProgram m_fragmentShader;
    Pipeline m_pipeline;
};

ENVY_NAMESPACE_END