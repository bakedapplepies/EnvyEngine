#pragma once

#include "Envy/EnvyInstance.h"
#include "Envy/opengl/VertexArray.h"
#include "Material.h"

struct RenderCommand
{
    const Envy::VertexArray* vertexArray;
    const Envy::VAOChunk* vaoChunk;
    const Material* material;
};

class Renderer
{
public:
    Renderer(const Envy::EnvyInstance* envy_instance);
    ~Renderer();

    void Render(const RenderCommand& render_command) const;

private:
    const Envy::EnvyInstance* m_envyInstance = nullptr;
};