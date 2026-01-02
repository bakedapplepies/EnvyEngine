#pragma once

#include <glm/glm.hpp>

#include "Envy/EnvyInstance.h"
#include "Envy/opengl/VertexArray.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"

struct RenderCommand
{
    const Envy::VertexArray* vertexArray;
    const Envy::VAOChunk* vaoChunk;
    const Material* material;
    const Transform* transform;
};

struct GlobalUBO
{
    float time;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec4 cameraPos;
};

class Renderer
{
public:
    Renderer(const Envy::EnvyInstance* envy_instance);
    ~Renderer();

    void Render(const Camera* camera,
                const Envy::Cubemap* cubemap,
                const RenderCommand& render_command) const;

private:
    const Envy::EnvyInstance* m_envyInstance = nullptr;
    const Envy::UniformBuffer* m_globalUBO = nullptr;
};