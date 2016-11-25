/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

const char *vsBasicLighting = R"(
    #version 330 core

    layout (location = 0) in vec4 position;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec2 texCoord0;

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

static const char* fsChecker = R"(
    #version 330 core

    uniform vec4 color;

    in vec2 uv0;
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