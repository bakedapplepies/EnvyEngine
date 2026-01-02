#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>

#include "Envy/core/Defines.h"
#include "Envy/opengl/ShaderProgram.h"
#include "Envy/opengl/Texture2D.h"
#include "Envy/opengl/VertexArray.h"
#include "Envy/opengl/Pipeline.h"
#include "Envy/opengl/UniformBuffer.h"
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
    const Texture2D* GetTexture2D(std::string_view file_path) const;
    
    const Texture2D* CreateTexture2DEmpty(TextureFormat texture_format,
                                          int width,
                                          int height);
    const VertexArray* CreateVAO(const Vertex* vertices, uint32_t n_vertices,
                                 const GLuint* indices, uint32_t n_indices);
    Pipeline* CreatePipeline();
    const UniformBuffer* CreateUBO(uint32_t ubo_block_size, uint32_t binding);
    const Cubemap* CreateCubemap(TextureFormat format,
                                 const std::array<std::string_view, 6>& texture2D_paths);

    static std::string ReadFile(std::string_view file_path);
    static uint8_t* ReadImage(std::string_view file_path,
                              int* width, int* height, int* channels,
                              bool flip = true);

private:
    std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;
    std::unordered_map<std::string, size_t> m_texture2DIndices;
    std::vector<Texture2D> m_texture2Ds;
    std::vector<VertexArray> m_VAOs;
    std::vector<Pipeline> m_pipelines;
    std::vector<UniformBuffer> m_UBOs;
    std::vector<Cubemap> m_cubemaps;
};

ENVY_NAMESPACE_END