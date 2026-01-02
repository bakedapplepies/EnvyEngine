#version 460 core

out vec4 FragColor;

const uint TEXTURE_UNIT_0 = 0;
const uint TEXTURE_UNIT_1 = 1;
const uint TEXTURE_UNIT_2 = 2;
const uint TEXTURE_UNIT_CUBEMAP = 3;

layout (location = 0) in V_OUT
{
    vec3 o_WorldPosition;
    vec3 o_Normal;
    vec2 o_UV;
} v_out;

// layout (binding = TEXTURE_UNIT_0) uniform sampler2D u_imageOutput;
layout (binding = TEXTURE_UNIT_0) uniform sampler2D u_albedo;

void main()
{
    // vec3 imageSample = texture(u_imageOutput, v_out.o_UV).xyz;
    // FragColor = vec4(imageSample, 0.0);

    vec3 albedoSample = texture(u_albedo, v_out.o_UV).xyz;

    FragColor = vec4(albedoSample, 1.0);
}   