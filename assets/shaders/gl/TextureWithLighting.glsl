// VERTEX

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

// FRAGMENT

#version 330 core

uniform sampler2D mainTex;

in vec2 uv0;
in vec3 n;

out vec4 fragColor;

void main()
{
    vec4 color = texture(mainTex, uv0);
    fragColor = color * dot(vec3(1, 1, 1), n) / (length(vec3(1, 1, 1)) * length(n));
}
