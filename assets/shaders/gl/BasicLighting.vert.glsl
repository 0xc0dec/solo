#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord0;

uniform mat4 worldViewProjMatrix;
uniform mat4 invTransposedWorldMatrix;
out vec2 uv0;
out vec3 n;

void main()
{
    gl_Position = worldViewProjMatrix * vec4(position, 1);
    uv0 = texCoord0;
    n = normalize((invTransposedWorldMatrix * vec4(normal, 1)).xyz);
}