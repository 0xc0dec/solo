// VERTEX

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

// FRAGMENT

#version 450

layout (binding = 1) uniform Variables
{
    vec4 color;
} variables;

layout (location = 0) in vec2 uv0;
layout (location = 0) out vec4 fragColor;

void main()
{
    float xfloor = floor(uv0.x / 0.2);
    float yfloor = floor(uv0.y / 0.2) + 1;
    float m = mod(xfloor, 2) + mod(yfloor, 2);
    if (m == 0 || m >= 2)
        fragColor = vec4(0, 0, 0, 1);
    else
        fragColor = variables.color;
}