#version 330 core

uniform sampler2D mainTex;

in vec2 uv0;
out vec4 fragColor;

void main()
{
    fragColor = texture(mainTex, uv0);
}