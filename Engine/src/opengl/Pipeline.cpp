#include "Envy/opengl/Pipeline.h"

ENVY_NAMESPACE_START

Pipeline::Pipeline()
{
    glCreateProgramPipelines(1, &m_pipelineID);
}

Pipeline::~Pipeline()
{
    glDeleteProgramPipelines(1, &m_pipelineID);

    m_pipelineID = GL_NONE;
}

Pipeline::Pipeline(Pipeline&& other) noexcept
{
    m_vertexProgram = other.m_vertexProgram;
    m_fragmentProgram = other.m_fragmentProgram;
    m_computeProgram = other.m_computeProgram;
    m_pipelineID = other.m_pipelineID;

    other.m_vertexProgram = nullptr;
    other.m_fragmentProgram = nullptr;
    other.m_computeProgram = nullptr;
    other.m_pipelineID = GL_NONE;
}

Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
{
    m_vertexProgram = other.m_vertexProgram;
    m_fragmentProgram = other.m_fragmentProgram;
    m_computeProgram = other.m_computeProgram;
    m_pipelineID = other.m_pipelineID;

    other.m_vertexProgram = nullptr;
    other.m_fragmentProgram = nullptr;
    other.m_computeProgram = nullptr;
    other.m_pipelineID = GL_NONE;

    return *this;
}

void Pipeline::SetVertexProgram(const ShaderProgram* program)
{
    m_vertexProgram = program;
    m_vertexProgram->AssignPipeline(m_pipelineID);
}

void Pipeline::SetFragmentProgram(const ShaderProgram* program)
{
    m_fragmentProgram = program;
    m_fragmentProgram->AssignPipeline(m_pipelineID);
}

void Pipeline::SetComputeProgram(const ShaderProgram* program)
{
    m_computeProgram = program;
    m_computeProgram->AssignPipeline(m_pipelineID);
}

const ShaderProgram* Pipeline::GetVertexProgram() const
{
    return m_vertexProgram;
}

const ShaderProgram* Pipeline::GetFragmentProgram() const
{
    return m_fragmentProgram;
}

const ShaderProgram* Pipeline::GetComputeProgram() const
{
    return m_computeProgram;
}

void Pipeline::Bind() const
{
    glBindProgramPipeline(m_pipelineID);
}

void Pipeline::BindCS() const
{
    m_computeProgram->Bind();
}

void Pipeline::UnbindCS() const
{
    glUseProgram(0);
}

ENVY_NAMESPACE_END