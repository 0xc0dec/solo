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