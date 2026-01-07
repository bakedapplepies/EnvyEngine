#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>

#include "Envy/core/Defines.h"
#include "Envy/util/ResourceVector.h"
#include "Envy/opengl/ShaderProgram.h"
#include "Envy/opengl/Texture2D.h"
#include "Envy/opengl/VertexArray.h"
#include "Envy/opengl/Pipeline.h"
#include "Envy/opengl/Framebuffer.h"
#include "Envy/opengl/UniformBuffer.h"
#include "Envy/opengl/IndirectBuffer.h"
#include "Envy/opengl/Cubemap.h"

ENVY_NAMESPACE_START

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void LoadShaderProgram(ShaderType shader_type, std::string_view file_path);
    void LoadTexture2D(TextureFormat texture_format, std::string_view file_path);
    const ShaderProgram* GetShaderProgram(std::string_view file_path) const;
    const Resource<Texture2D> GetTexture2D(std::string_view file_path) const;
    
    const Resource<Texture2D> CreateTexture2DEmpty(TextureFormat texture_format,
                                                   int width,
                                                   int height);
    const Resource<VertexArray> CreateVAO(const Vertex* vertices, uint32_t n_vertices,
                                          const GLuint* indices, uint32_t n_indices);
    Resource<Pipeline> CreatePipeline();
    Resource<Pipeline> CreatePipeline(const ShaderProgram* vert_program, const ShaderProgram* frag_program);
    const Resource<Framebuffer> CreateFramebuffer(int width, int height);
    const Resource<UniformBuffer> CreateUBO(uint32_t ubo_block_size, uint32_t binding);
    const Resource<IndirectBuffer> CreateIndirectBuffer(uint32_t command_count,
                                               const IndirectCommand* commands);
    const Resource<Cubemap> CreateCubemap(TextureFormat format,
                                 const std::array<std::string_view, 6>& texture2D_paths);

    static std::string ReadFile(std::string_view file_path);
    static uint8_t* ReadImage(std::string_view file_path,
                              int* width, int* height, int* channels,
                              bool flip = true);

private:
    std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;
    std::unordered_map<std::string, size_t> m_texture2DIndices;
    ResourceVector<Texture2D> m_texture2Ds;
    ResourceVector<VertexArray> m_VAOs;
    ResourceVector<Pipeline> m_pipelines;
    ResourceVector<Framebuffer> m_framebuffers;
    ResourceVector<UniformBuffer> m_UBOs;
    ResourceVector<IndirectBuffer> m_indirectBuffers;
    ResourceVector<Cubemap> m_cubemaps;
};

ENVY_NAMESPACE_END