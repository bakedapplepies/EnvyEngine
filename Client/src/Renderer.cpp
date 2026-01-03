#include "Renderer.h"

#include <GLFW/glfw3.h>

Renderer::Renderer(const Envy::EnvyInstance* envy_instance)
{
    m_envyInstance->SetFrontFaceOrder(Envy::FaceOrder::CLOCKWISE);
    m_envyInstance->SetDepthTesting(true);

    m_envyInstance = envy_instance;

    m_globalUBO = m_envyInstance->CreateUBO(160, 0);
}

Renderer::~Renderer()
{
    m_envyInstance = nullptr;
    m_globalUBO = nullptr;
}

void Renderer::Render(const Camera* camera, 
                      const Envy::Cubemap* cubemap,
                      const RenderCommand& render_command) const
{
    const Material* material = render_command.material;

    const uint32_t TEXTURE_UNIT_ALBEDO = 0;
    material->albedo->Bind(TEXTURE_UNIT_ALBEDO);

    material->pipeline->Bind();
    material->pipeline->GetVertexProgram()->UniformMat4("u_model",
                                                        render_command.transform->GetModelMatrix());

    // Update only once
    _UpdateGlobalUBO(camera);

    render_command.vertexArray->Bind();

    m_envyInstance->Draw(*render_command.vaoChunk);

    if (cubemap)  // Rendered last to avoid over-draw
    {
        cubemap->Draw();
    }
}

void Renderer::RenderIndirect(const Camera* camera,
                              const Envy::Cubemap* cubemap,
                              const RenderCommandIndirect& render_command_indirect) const
{
    const Material* material = render_command_indirect.material;

    const uint32_t TEXTURE_UNIT_ALBEDO = 0;
    material->albedo->Bind(TEXTURE_UNIT_ALBEDO);

    material->pipeline->Bind();
    material->pipeline->GetVertexProgram()->UniformMat4("u_model",
                                                        render_command_indirect.transform->GetModelMatrix());

    // Update only once
    _UpdateGlobalUBO(camera);

    render_command_indirect.vertexArray->Bind();
    render_command_indirect.indirectBuffer->Bind();

    m_envyInstance->DrawIndirect(render_command_indirect.indirectBuffer->GetCommandCount());

    if (cubemap)  // Rendered last to avoid over-draw
    {
        cubemap->Draw();
    }
}

void Renderer::_UpdateGlobalUBO(const Camera* camera) const
{
    float time = glfwGetTime();
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glm::mat4 projectionMatrix = camera->GetProjectionMatrix(16.0f/9.0f);
    m_globalUBO->UploadData(0, 4, &time);
    m_globalUBO->UploadData(16, 64, &viewMatrix);
    m_globalUBO->UploadData(80, 64, &projectionMatrix);
    m_globalUBO->UploadData(144, 16, &camera->position);
}