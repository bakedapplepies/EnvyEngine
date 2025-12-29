#pragma once

#include <string_view>

#include <glad/glad.h>

#include "Envy/core/Defines.h"
#include "Envy/Envy.h"

ENVY_NAMESPACE_START

class ShaderProgram
{
public:
    ShaderProgram() = default;
    ShaderProgram(ShaderType shader_type, std::string_view source);
    ~ShaderProgram();
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    ShaderType GetShaderType() const;
    void AssignPipeline(GLuint pipelineID) const;
    void Bind() const;

    void UniformInt(std::string_view uniform_name, GLint value) const;
    void UniformFloat(std::string_view uniform_name, GLfloat value) const;
    void UniformMat4(std::string_view uniform_name, glm::mat4 value) const;

private:
    ShaderType m_shaderType = ShaderType::NONE;
    GLuint m_programID = 0;
};

ENVY_NAMESPACE_END