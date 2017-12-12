#version 450

layout (binding = 1) uniform Variables
{
    vec4 color;
} variables;

layout (location = 0) out vec3 outColor;

void main()
{
    outColor = variables.color.xyz;
}