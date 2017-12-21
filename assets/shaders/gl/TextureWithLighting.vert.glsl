#version 330 core

uniform mat4 wvp;
uniform mat4 invTranspWorld;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord0;

out vec2 uv0;
out vec3 n;

void main()
{
    gl_Position = wvp * vec4(position, 1);
    uv0 = texCoord0;
    n = normalize((invTranspWorld * vec4(normal, 1)).xyz);
}