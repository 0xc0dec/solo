#version 450

layout (binding = 0) uniform UBO
{
    vec4 someColor;
} ubo;

layout (location = 0) smooth in vec3 inColor;

layout (location = 0) out vec4 outFragColor;

void main()
{
	outFragColor = ubo.someColor;
}