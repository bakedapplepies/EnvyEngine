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

layout (location = 0) out V_OUT
{
    vec3 o_UV;
} v_out;

layout (binding = 0, std140) uniform GlobalUBO
{
    float u_time;
    mat4 u_view;
    mat4 u_projection;
    vec4 u_cameraPos;
};

void main()
{
    v_out.o_UV = vec3(i_Position.x, i_Position.y, i_Position.z);

    // remove the translation part of the view matrix
    vec4 pos = u_projection * mat4(mat3(u_view)) * vec4(i_Position, 1.0);

    gl_Position = pos.xyww;  // this is so that the resulting depth value will
                             // always be 1.0 (perspective division)
}