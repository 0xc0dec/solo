#version 450

layout (binding = 1) uniform sampler2D colorTex;
layout (binding = 2) uniform Test
{
    vec4 v4;
    vec3 v3;
    vec2 v2;
    float f;
} test;

layout (location = 0) in vec2 inTexCoord;

layout (location = 0) out vec3 outFragColor;

void main()
{
    vec4 color = texture(colorTex, inTexCoord, 1);
    outFragColor = color.rgb * test.v3 * vec3(test.v2, 1) * vec3(test.v4.xyz) * test.f;
}