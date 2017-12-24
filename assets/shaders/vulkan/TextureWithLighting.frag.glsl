#version 450

layout (binding = 1) uniform sampler2D mainTex;

layout (location = 0) in vec2 uv0;
layout (location = 1) in vec3 n;

layout (location = 0) out vec4 fragColor;

void main()
{
    vec4 color = texture(mainTex, uv0);
    fragColor = color * dot(vec3(1, 1, 1), n) / (length(vec3(1, 1, 1)) * length(n));
}