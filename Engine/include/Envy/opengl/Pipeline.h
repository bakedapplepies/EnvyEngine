#pragma once

#include <glad/glad.h>

#include "Envy/core/Defines.h"
#include "Envy/opengl/ShaderProgram.h"

ENVY_NAMESPACE_START

class Pipeline
{
public:
    Pipeline();
    ~Pipeline();
    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&& other) noexcept;
    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline& operator=(Pipeline&& other) noexcept;

    void SetVertexProgram(const ShaderProgram* program);
    void SetFragmentProgram(const ShaderProgram* program);
    void SetComputeProgram(const ShaderProgram* program);

    const ShaderProgram* GetVertexProgram() const;
    const ShaderProgram* GetFragmentProgram() const;
    const ShaderProgram* GetComputeProgram() const;

    void Bind() const;
    void BindCS() const;
    void UnbindCS() const;

    void UniformInt(std::string_view uniform_name, GLint value) const;
    void UniformFloat(std::string_view uniform_name, GLfloat value) const;

private:
    const ShaderProgram* m_vertexProgram = nullptr;
    const ShaderProgram* m_fragmentProgram = nullptr;
    const ShaderProgram* m_computeProgram = nullptr;
    GLuint m_pipelineID = GL_NONE;
};

ENVY_NAMESPACE_END