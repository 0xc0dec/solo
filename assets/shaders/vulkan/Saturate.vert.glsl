#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord0;

layout (location = 0) out vec2 uv0;

void main()
{
    gl_Position = position;
    uv0 = texCoord0;
}