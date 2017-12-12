#version 450

layout (binding = 0) uniform Matrices
{
    mat4 wvp;
} matrices;

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 0) out vec2 outTexCood;

void main()
{
    outTexCood = inTexCoord;
	gl_Position = matrices.wvp * vec4(inPos.xyz, 1.0);
    gl_Position.y = -gl_Position.y;
}
