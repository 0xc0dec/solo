// VERTEX

#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord0;

out vec2 uv0;

void main()
{
    gl_Position = position;
    uv0 = texCoord0;
}

// FRAGMENT

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