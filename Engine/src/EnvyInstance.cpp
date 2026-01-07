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

void EnvyInstance::SetFrontFaceOrder(FaceOrder face_order) const
{
    glEnable(GL_CULL_FACE);
    glFrontFace(static_cast<GLenum>(face_order));
    glCullFace(GL_BACK);
}

void EnvyInstance::SetDepthTesting(bool enable) const
{
    if (enable)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
    else glDisable(GL_DEPTH_TEST);
}

void EnvyInstance::BindDefaultFramebuffer() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

const Resource<Texture2D> EnvyInstance::GetTexture2D(std::string_view file_path) const
{
    return m_GLResourceManager->GetTexture2D(file_path);
}

const Resource<Texture2D> EnvyInstance::CreateTexture2DEmpty(TextureFormat texture_format,
                                                    int width,
                                                    int height) const
{
    return m_GLResourceManager->CreateTexture2DEmpty(texture_format, width, height);
}

const Resource<VertexArray> EnvyInstance::CreateVertexArray(const Vertex* vertices, uint32_t n_vertices,
                                                   const GLuint* indices, uint32_t n_indices) const
{
    return m_GLResourceManager->CreateVAO(vertices, n_vertices, indices, n_indices);
}

Resource<Pipeline> EnvyInstance::CreatePipeline() const
{
    return m_GLResourceManager->CreatePipeline();
}

Resource<Pipeline> EnvyInstance::CreatePipeline(const ShaderProgram* vert_program,
                                       const ShaderProgram* frag_program) const
{
    return m_GLResourceManager->CreatePipeline(vert_program, frag_program);
}

const Resource<Framebuffer> EnvyInstance::CreateFramebuffer(int width, int height) const
{
    return m_GLResourceManager->CreateFramebuffer(width, height);
}

const Resource<UniformBuffer> EnvyInstance::CreateUBO(uint32_t ubo_block_size, uint32_t binding) const
{
    return m_GLResourceManager->CreateUBO(ubo_block_size, binding);
}

const Resource<IndirectBuffer> EnvyInstance::CreateIndirectBuffer(uint32_t command_count,
                                                                  const IndirectCommand* commands) const
{
    return m_GLResourceManager->CreateIndirectBuffer(command_count, commands);
}

const Resource<Cubemap> EnvyInstance::CreateCubemap(TextureFormat format,
                                                    std::string_view right,
                                                    std::string_view left,
                                                    std::string_view top,
                                                    std::string_view bottom,
                                                    std::string_view front,
                                                    std::string_view back) const
{
    std::array<std::string_view, 6> texture2DPaths {
        right, left, top, bottom, front, back
    };
    return m_GLResourceManager->CreateCubemap(format, texture2DPaths);
}

void EnvyInstance::Draw(const VAOChunk& vao_chunk) const
{
    glDrawElementsBaseVertex(GL_TRIANGLES,
                             vao_chunk.elementsCount,
                             GL_UNSIGNED_INT,
                             reinterpret_cast<void*>(vao_chunk.elementsOffset * sizeof(float)),
                             vao_chunk.vertexOffset);
}

void EnvyInstance::DrawIndirect(uint32_t indirect_count) const
{
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, indirect_count, 0);
}

ENVY_NAMESPACE_END
