// VERTEX

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

// FRAGMENT

#version 330 core

uniform sampler2D mainTex;

in vec2 uv0;
out vec4 fragColor;

void main()
{
    fragColor = texture(mainTex, uv0);
}
