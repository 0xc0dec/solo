#version 450

layout (binding = 0) uniform Matrices
{
    mat4 wvp;
} matrices;

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord0;

layout (location = 0) out vec2 uv0;

void main()
{
    uv0 = texCoord0;
    gl_Position = matrices.wvp * position;
    gl_Position.y = -gl_Position.y;
}