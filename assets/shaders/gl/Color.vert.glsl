#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord0;

uniform mat4 wvp;
out vec2 uv0;

void main()
{
    gl_Position = wvp * position;
    uv0 = texCoord0;
}