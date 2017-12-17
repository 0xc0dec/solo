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