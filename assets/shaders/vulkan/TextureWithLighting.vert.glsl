#version 450

layout (binding = 0) uniform Matrices
{
    mat4 wvp;
    mat4 invTranspWorld;
} matrices;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord0;

layout (location = 0) out vec2 uv0;
layout (location = 1) out vec3 n;

void main()
{
    uv0 = texCoord0;
    uv0.y = -uv0.y;
    n = normalize((matrices.invTranspWorld * vec4(normal, 1)).xyz);
    gl_Position = matrices.wvp * vec4(position, 1);
    gl_Position.y = -gl_Position.y;
}