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

    void LoadShaderProgram(ShaderType shader_type, std::string_view file_path) const;
    void LoadTexture2D(TextureFormat texture_format, std::string_view file_path) const;
    const ShaderProgram* GetShaderProgram(std::string_view file_path) const;
    const Texture2D* GetTexture2D(std::string_view file_path) const;
    
    const Texture2D* CreateTexture2DEmpty(TextureFormat texture_format, int width, int height) const;
    const VertexArray* CreateVertexArray(const Vertex* vertices, uint32_t n_vertices,
                                        const GLuint* indices, uint32_t n_indices) const;
    Pipeline* CreatePipeline() const;

private:
    bool m_activeInstance = false;
    std::unique_ptr<ResourceManager> m_GLResourceManager;
};

ENVY_NAMESPACE_END