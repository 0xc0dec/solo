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
