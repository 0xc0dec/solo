#version 330 core

uniform vec4 color;

in vec2 uv0;
out vec4 fragColor;

void main()
{
    fragColor = color;
}