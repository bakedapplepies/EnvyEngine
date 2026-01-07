#include "Envy/opengl/Pipeline.h"

ENVY_NAMESPACE_START

Pipeline::Pipeline()
{
    glCreateProgramPipelines(1, &m_pipelineID);
}

Pipeline::Pipeline(const ShaderProgram* vert_program, const ShaderProgram* frag_program)
{
    glCreateProgramPipelines(1, &m_pipelineID);
    SetVertexProgram(vert_program);
    SetFragmentProgram(frag_program);
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
    m_pipelineID = other.m_pipelineID;

    other.m_vertexProgram = nullptr;
    other.m_fragmentProgram = nullptr;
    other.m_pipelineID = GL_NONE;
}

Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
{
    m_vertexProgram = other.m_vertexProgram;
    m_fragmentProgram = other.m_fragmentProgram;
    m_pipelineID = other.m_pipelineID;

    other.m_vertexProgram = nullptr;
    other.m_fragmentProgram = nullptr;
    other.m_pipelineID = GL_NONE;

    return *this;
}

void Pipeline::             SetVertexProgram(const ShaderProgram* program)
{
    m_vertexProgram = program;
    m_vertexProgram->AssignPipeline(m_pipelineID);
}

void Pipeline::SetFragmentProgram(const ShaderProgram* program)
{
    m_fragmentProgram = program;
    m_fragmentProgram->AssignPipeline(m_pipelineID);
}

const ShaderProgram* Pipeline::GetVertexProgram() const
{
    return m_vertexProgram;
}

const ShaderProgram* Pipeline::GetFragmentProgram() const
{
    return m_fragmentProgram;
}

void Pipeline::Bind() const
{
    glBindProgramPipeline(m_pipelineID);
}

ENVY_NAMESPACE_END