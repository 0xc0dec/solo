#version 450

layout (binding = 1) uniform sampler2D colorTex;

layout (location = 0) in vec2 inTexCoord;

layout (location = 0) out vec3 outFragColor;

void main()
{
    vec4 color = texture(colorTex, inTexCoord, 1);
    outFragColor = color.rgb;
}