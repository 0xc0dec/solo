// VERTEX

#version 450

layout (binding = 0) uniform Matrices
{
    mat4 wvp;
} matrices;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord0;

layout (location = 0) out vec2 outTexCood;

void main()
{
    outTexCood = texCoord0;
    outTexCood.y = 1 - outTexCood.y;
	gl_Position = matrices.wvp * vec4(position.xyz, 1.0);
    gl_Position.y = -gl_Position.y;
}

// FRAGMENT

#version 450

layout (binding = 1) uniform sampler2D mainTex;

layout (location = 0) in vec2 inTexCoord;

layout (location = 0) out vec4 outFragColor;

void main()
{
    outFragColor = texture(mainTex, inTexCoord);
}