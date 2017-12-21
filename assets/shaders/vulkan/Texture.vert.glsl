#version 450

layout (binding = 0) uniform Matrices
{
    mat4 wvp;
} matrices;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord0;

layout (location = 0) out vec2 outTexCood;

void main()
{
    outTexCood = texCoord0;
    outTexCood.y = -outTexCood.y;
	gl_Position = matrices.wvp * vec4(position.xyz, 1.0);
    gl_Position.y = -gl_Position.y;
}
