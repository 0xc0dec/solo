// VERTEX

#version 450

layout (binding = 0) uniform Matrices
{
    mat4 wvp;
} matrices;

layout (location = 0) in vec3 inPos;

void main()
{
	gl_Position = matrices.wvp * vec4(inPos.xyz, 1.0);
    gl_Position.y = -gl_Position.y;
}

// FRAGMENT

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
