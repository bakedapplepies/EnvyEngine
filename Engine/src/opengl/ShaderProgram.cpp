#include "Envy/opengl/ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include "Envy/core/Assert.h"

ENVY_NAMESPACE_START

ShaderProgram::ShaderProgram(ShaderType shader_type, std::string_view source)
{
    const char* shaderSource = source.data();
    GLenum shaderType = static_cast<GLenum>(shader_type);

    m_programID = glCreateShaderProgramv(shaderType, 1, &shaderSource);
    m_shaderType = shader_type;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_programID);

    m_programID = 0;
    m_shaderType = ShaderType::NONE;
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
{
    m_programID = other.m_programID;
    m_shaderType = other.m_shaderType;

    other.m_programID = 0;
    other.m_shaderType = ShaderType::NONE;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    m_programID = other.m_programID;
    m_shaderType = other.m_shaderType;

    other.m_programID = 0;
    other.m_shaderType = ShaderType::NONE;

    return *this;
}

ShaderType ShaderProgram::GetShaderType() const
{
    return m_shaderType;
}

void ShaderProgram::AssignPipeline(GLuint pipelineID) const
{
    GLbitfield stage = GL_NONE;
    if (m_shaderType == ShaderType::VERTEX) stage = GL_VERTEX_SHADER_BIT;
    else if (m_shaderType == ShaderType::FRAGMENT) stage = GL_FRAGMENT_SHADER_BIT;
    else if (m_shaderType == ShaderType::COMPUTE) stage = GL_COMPUTE_SHADER_BIT;

    ENVY_ASSERT(stage != GL_NONE, "Unknown shader stage.");

    const void* a = (uintptr_t*)1 + 2;

    glUseProgramStages(pipelineID, stage, m_programID);
}

void ShaderProgram::Bind() const
{
    glUseProgram(m_programID);
}

void ShaderProgram::UniformInt(std::string_view uniform_name, GLint value) const
{
    GLint location = glGetUniformLocation(m_programID, uniform_name.data());
    glProgramUniform1i(m_programID, location, value);
}

void ShaderProgram::UniformFloat(std::string_view uniform_name, GLfloat value) const
{
    GLint location = glGetUniformLocation(m_programID, uniform_name.data());
    glProgramUniform1f(m_programID, location, value);
}

void ShaderProgram::UniformMat4(std::string_view uniform_name, glm::mat4 value) const
{
    GLint location = glGetUniformLocation(m_programID, uniform_name.data());
    glProgramUniformMatrix4fv(m_programID, location, 1, GL_FALSE, glm::value_ptr(value));
}

ENVY_NAMESPACE_END