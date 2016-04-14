#pragma once

const char* vsBasicLighting = R"(
    #version 330 core

    in vec4 position;
	in vec3 normal;
	in vec2 texCoord0;

    uniform mat4 worldViewProjMatrix;
	uniform mat4 invTransposedWorldMatrix;
	out vec2 uv0;
	out vec3 n;

    void main()
	{
		gl_Position = worldViewProjMatrix * position;
		uv0 = texCoord0;
		n = normalize((invTransposedWorldMatrix * vec4(normal, 1)).xyz);
	}
)";

const char* fsColor = R"(
    #version 330 core

    uniform vec4 color;

    in vec2 uv0;
	out vec4 fragColor;

    void main()
	{
		fragColor = color;
	}
)";

static const char* fsChecker = R"(
    #version 330 core

    in vec2 uv0;
	uniform vec4 color;
	out vec4 fragColor;

    void main()
	{
		float xfloor = floor(uv0.x / 0.2);
		float yfloor = floor(uv0.y / 0.2) + 1;
		float m = mod(xfloor, 2) + mod(yfloor, 2);
		if (m == 0 || m >= 2)
			fragColor = vec4(0, 0, 0, 1);
		else
			fragColor = color;
	}
)";

static const char* fsTextureWithLighting = R"(
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
)";