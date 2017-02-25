#version 330 core

uniform sampler2D mainTex;
uniform float leftSeparator;
uniform float rightSeparator;

in vec2 uv0;
out vec4 fragColor;

void main()
{
    vec4 color = texture(mainTex, uv0);
    if (uv0.x >= leftSeparator && uv0.x <= rightSeparator)
        color *= 2;
    fragColor = color;
}