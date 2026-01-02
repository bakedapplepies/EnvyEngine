#version 460 core

out vec4 FragColor;

const uint TEXTURE_UNIT_0 = 0;
const uint TEXTURE_UNIT_1 = 1;
const uint TEXTURE_UNIT_2 = 2;
const uint TEXTURE_UNIT_CUBEMAP = 3;

layout (location = 0) in V_OUT
{
    vec3 o_UV;
} v_out;

layout (binding = TEXTURE_UNIT_CUBEMAP) uniform samplerCube u_cubemap;

void main()
{
    FragColor = texture(u_cubemap, v_out.o_UV);
    // FragColor = vec4(1.0);
}