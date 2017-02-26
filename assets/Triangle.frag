#version 450

layout (location = 0) smooth in vec3 inColor;

layout (location = 0) out vec3 outFragColor;

void main()
{
	outFragColor = inColor;
}