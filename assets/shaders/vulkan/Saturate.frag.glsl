#version 450

layout (binding = 0) uniform Variables
{
    float leftSeparator;
    float rightSeparator;
} variables;

layout (binding = 1) uniform sampler2D mainTex;

layout (location = 0) in vec2 uv0;

layout (location = 0) out vec4 fragColor;

void main()
{
    vec4 color = texture(mainTex, uv0);
    if (uv0.x >= variables.leftSeparator && uv0.x <= variables.rightSeparator)
        color *= 2;
    fragColor = color;
}