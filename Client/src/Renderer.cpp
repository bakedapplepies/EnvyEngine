#include "Renderer.h"

Renderer::Renderer(const Envy::EnvyInstance* envy_instance)
{
    m_envyInstance = envy_instance;
}

Renderer::~Renderer()
{
    m_envyInstance = nullptr;
}

void Renderer::Render(const RenderCommand& render_command) const
{
    const uint32_t TEXTURE_UNIT_ALBEDO = 0;
    render_command.material->albedo->Bind(TEXTURE_UNIT_ALBEDO);

    render_command.material->pipeline->Bind();

    render_command.vertexArray->Bind();

    glDrawElementsBaseVertex(GL_TRIANGLES,
                             render_command.vaoChunk->elementsCount,
                             GL_UNSIGNED_INT,
                             reinterpret_cast<void*>(render_command.vaoChunk->elementsOffset * sizeof(float)),
                             render_command.vaoChunk->vertex_offset);
}