#include "Envy/EnvyInstance.h"

#include <fmt/format.h>

#include "Envy/core/Debug.h"

// TODO: GLFW is probably doing some allocations in the background
//       which the OS hasn't bothered to clean up by the time we're done.
#ifndef MLC_ASAN_DETECT_LEAKS
#   ifdef __cplusplus
extern "C"
#   endif
const char* __asan_default_options() { return "detect_leaks=0"; }
#endif

ENVY_NAMESPACE_START

EnvyInstance::EnvyInstance()
{}

EnvyInstance::~EnvyInstance()
{}

bool EnvyInstance::Init()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugMessageCallback, nullptr);

    m_activeInstance = true;
    m_GLResourceManager = std::make_unique<ResourceManager>();

    return true;
}

void EnvyInstance::Shutdown()
{
    m_activeInstance = false;
    m_GLResourceManager.reset();
}

void EnvyInstance::ClearBuffer() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EnvyInstance::ClearColor(float r, float g, float b, float a) const
{
    glClearColor(r, g, b, a);
}

void EnvyInstance::SetViewport(int x, int y, int dx, int dy) const
{
    glViewport(x, y, dx, dy);
}

void EnvyInstance::LoadShaderProgram(ShaderType shader_type, std::string_view file_path) const
{
    m_GLResourceManager->LoadShaderProgram(shader_type, file_path);
}

void EnvyInstance::LoadTexture2D(TextureFormat texture_format, std::string_view file_path) const
{
    m_GLResourceManager->LoadTexture2D(texture_format, file_path);
}

const ShaderProgram* EnvyInstance::GetShaderProgram(std::string_view file_path) const
{
    return m_GLResourceManager->GetShaderProgram(file_path);
}

const Texture2D* EnvyInstance::GetTexture2D(std::string_view file_path) const
{
    return m_GLResourceManager->GetTexture2D(file_path);
}

const Texture2D* EnvyInstance::CreateTexture2DEmpty(TextureFormat texture_format,
                                                    int width,
                                                    int height) const
{
    return m_GLResourceManager->CreateTexture2DEmpty(texture_format, width, height);
}

const VertexArray* EnvyInstance::CreateVertexArray(const Vertex* vertices, uint32_t n_vertices,
                                                   const GLuint* indices, uint32_t n_indices) const
{
    return m_GLResourceManager->CreateVAO(vertices, n_vertices, indices, n_indices);
}

Pipeline* EnvyInstance::CreatePipeline() const
{
    return m_GLResourceManager->CreatePipeline();
}

ENVY_NAMESPACE_END
