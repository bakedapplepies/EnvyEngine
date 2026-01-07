#pragma once

#include <glad/glad.h>

#include <memory>

#include "Envy/core/Defines.h"
#include "Envy/opengl/ResourceManager.h"

ENVY_NAMESPACE_START

class EnvyInstance
{
public:
    EnvyInstance();
    ~EnvyInstance();

    bool Init();
    void Shutdown();
    
    void ClearBuffer() const;
    void ClearColor(float r, float g, float b, float a) const;
    void SetViewport(int x, int y, int dx, int dy) const;
    void SetFrontFaceOrder(FaceOrder face_order) const;
    void SetDepthTesting(bool enable) const;
    void BindDefaultFramebuffer() const;

    void LoadShaderProgram(ShaderType shader_type, std::string_view file_path) const;
    void LoadTexture2D(TextureFormat texture_format, std::string_view file_path) const;
    ENVY_NODISCARD const ShaderProgram* GetShaderProgram(std::string_view file_path) const;
    ENVY_NODISCARD const Resource<Texture2D> GetTexture2D(std::string_view file_path) const;
    
    ENVY_NODISCARD const Resource<Texture2D> CreateTexture2DEmpty(TextureFormat texture_format,
                                                                  int width, int height) const;
    ENVY_NODISCARD const Resource<VertexArray> CreateVertexArray(const Vertex* vertices, uint32_t n_vertices,
                                                                 const GLuint* indices, uint32_t n_indices) const;
    ENVY_NODISCARD Resource<Pipeline> CreatePipeline() const;
    ENVY_NODISCARD Resource<Pipeline> CreatePipeline(const ShaderProgram* vert_program,
                                                     const ShaderProgram* frag_program) const;
    ENVY_NODISCARD const Resource<Framebuffer> CreateFramebuffer(int width, int height) const;
    ENVY_NODISCARD const Resource<UniformBuffer> CreateUBO(uint32_t ubo_block_size, uint32_t binding) const;
    ENVY_NODISCARD const Resource<IndirectBuffer> CreateIndirectBuffer(uint32_t command_count,
                                                                       const IndirectCommand* commands) const;
    ENVY_NODISCARD const Resource<Cubemap> CreateCubemap(TextureFormat format,
                                                         std::string_view right,
                                                         std::string_view left,
                                                         std::string_view top,
                                                         std::string_view bottom,
                                                         std::string_view front,
                                                         std::string_view back) const;

    void ConstructIndirectCommand() const;

    void Draw(const VAOChunk& vao_chunk) const;
    void DrawIndirect(uint32_t indirect_count) const;

private:
    bool m_activeInstance = false;
    std::unique_ptr<ResourceManager> m_GLResourceManager;
};

ENVY_NAMESPACE_END