#include "Envy/opengl/Cubemap.h"

#include "Envy/opengl/ResourceManager.h"

ENVY_NAMESPACE_START

Cubemap::Cubemap(int width,
                 int height,
                 TextureFormat format,
                 const std::array<uint8_t*, 6>& data,
                 const Sampler3DConfig& sampler3DConfig)
{
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_cubemapID);
    glTextureParameteri(m_cubemapID, GL_TEXTURE_WRAP_S, sampler3DConfig.wrapX);
    glTextureParameteri(m_cubemapID, GL_TEXTURE_WRAP_T, sampler3DConfig.wrapY);
    glTextureParameteri(m_cubemapID, GL_TEXTURE_WRAP_R, sampler3DConfig.wrapZ);
    glTextureParameteri(m_cubemapID, GL_TEXTURE_MIN_FILTER, sampler3DConfig.closeFilter);
    glTextureParameteri(m_cubemapID, GL_TEXTURE_MAG_FILTER, sampler3DConfig.farFilter);

    glTextureStorage2D(m_cubemapID, 1, static_cast<GLenum>(format), width, height);
    for (uint32_t face = 0; face < 6; face++)
    {
        glTextureSubImage3D(m_cubemapID,
                            0,
                            0, 0, face,
                            width, height, 1,
                            GL_RGBA,
                            GL_UNSIGNED_BYTE,
                            data[face]);
    }

    glBindTextureUnit(3, m_cubemapID);

    std::array<glm::vec3, 24> verticesPos = {
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),

        // right
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f,  1.0f),

        // back
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),

        // left
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),

        // top
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),

        // bottom
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
    };

    std::array<GLuint, 36> indices = {
        // front
        0, 2, 1,
        0, 3, 2,

        // right
        4, 6, 5,
        4, 7, 6,

        // back
        8, 10, 9,
        8, 11, 10,

        // left
        12, 14, 13,
        12, 15, 14,

        // top
        16, 18, 17,
        16, 19, 18,

        // back
        20, 22, 21,
        20, 23, 22
    };

    const GLuint VBO_BIND_INDEX_PER_VERTEX = 0;

    glCreateBuffers(1, &m_vboID);
    glNamedBufferStorage(m_vboID,
                         verticesPos.size() * sizeof(glm::vec3),
                         verticesPos.data(),
                         GL_DYNAMIC_STORAGE_BIT);
                         
    glCreateBuffers(1, &m_eboID);
    glNamedBufferStorage(m_eboID,
                         indices.size() * sizeof(GLuint),
                         indices.data(),
                         GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &m_vaoID);
    glVertexArrayVertexBuffer(m_vaoID, VBO_BIND_INDEX_PER_VERTEX, m_vboID, 0, sizeof(glm::vec3));
    glVertexArrayElementBuffer(m_vaoID, m_eboID);

    glEnableVertexArrayAttrib(m_vaoID, 0);

    glVertexArrayAttribFormat(m_vaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);

    m_vertexShader = ShaderProgram(ShaderType::VERTEX, ResourceManager::ReadFile("../../Engine/src/shaders/cubemaps/cubemap.vert"));
    m_fragmentShader = ShaderProgram(ShaderType::FRAGMENT, ResourceManager::ReadFile("../../Engine/src/shaders/cubemaps/cubemap.frag"));
    m_pipeline.SetVertexProgram(&m_vertexShader);
    m_pipeline.SetFragmentProgram(&m_fragmentShader);
}

Cubemap::~Cubemap()
{
    glDeleteTextures(1, &m_cubemapID);
    glDeleteVertexArrays(1, &m_vaoID);
    glDeleteBuffers(1, &m_vboID);
    glDeleteBuffers(1, &m_eboID);

    m_cubemapID = GL_NONE;
    m_vaoID = GL_NONE;
    m_vboID = GL_NONE;
    m_eboID = GL_NONE;
    m_format = TextureFormat::NONE;
}

Cubemap::Cubemap(Cubemap&& other) noexcept
{
    m_cubemapID = other.m_cubemapID;
    m_vboID = other.m_vboID;
    m_eboID = other.m_eboID;
    m_vaoID = other.m_vaoID;
    m_format = other.m_format;
    m_vertexShader = std::move(other.m_vertexShader);
    m_fragmentShader = std::move(other.m_fragmentShader);
    m_pipeline = std::move(other.m_pipeline);

    other.m_cubemapID = GL_NONE;
    other.m_vboID = GL_NONE;
    other.m_eboID = GL_NONE;
    other.m_vaoID = GL_NONE;
    other.m_format = TextureFormat::NONE;
}

Cubemap& Cubemap::operator=(Cubemap&& other) noexcept
{
    m_cubemapID = other.m_cubemapID;
    m_vboID = other.m_vboID;
    m_eboID = other.m_eboID;
    m_vaoID = other.m_vaoID;
    m_format = other.m_format;
    m_vertexShader = std::move(other.m_vertexShader);
    m_fragmentShader = std::move(other.m_fragmentShader);
    m_pipeline = std::move(other.m_pipeline);

    other.m_cubemapID = GL_NONE;
    other.m_vboID = GL_NONE;
    other.m_eboID = GL_NONE;
    other.m_vaoID = GL_NONE;
    other.m_format = TextureFormat::NONE;

    return *this;
}

void Cubemap::Draw() const
{
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(m_vaoID);
    m_pipeline.Bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDepthFunc(GL_LESS);
}

ENVY_NAMESPACE_END