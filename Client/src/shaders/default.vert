#version 460 core

// Re-declare built-in variables for stricter interface
// due to more flexible pipelines being able to choose different
// shader programs.
out gl_PerVertex
{
    vec4 gl_Position;
    // float gl_PointSize;
    // float gl_ClipDistance[];
};

layout (location = 0) in vec3 i_Position;
layout (location = 1) in vec3 i_Normal;
layout (location = 2) in vec2 i_UV;

layout (location = 0) out V_OUT
{
    vec3 o_Position;
    vec3 o_Normal;
    vec2 o_UV;
} v_out;

void main()
{
    v_out.o_Position = i_Position;
    v_out.o_Normal = i_Normal;
    v_out.o_UV = i_UV;

    gl_Position = vec4(i_Position, 1.0);
}