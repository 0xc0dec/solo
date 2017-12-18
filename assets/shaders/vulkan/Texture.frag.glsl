#version 450

layout (binding = 1) uniform sampler2D colorTex;

layout (location = 0) in vec2 inTexCoord;

layout (location = 0) out vec4 outFragColor;

void main()
{
    outFragColor = texture(colorTex, inTexCoord, 1);
}