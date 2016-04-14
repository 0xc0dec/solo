#pragma once


static const char* fsGrayscale = R"(
    #version 330 core

    uniform sampler2D mainTex;
	uniform float leftSeparator;
	uniform float rightSeparator;

    in vec2 uv0;
	out vec4 fragColor;

    void main()
	{
		vec4 color = texture(mainTex, uv0);
		if (uv0.x >= leftSeparator && uv0.x <= rightSeparator)
		{
			float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
			color = vec4(gray, gray, gray, 1);
		}
		fragColor = color;
	}
)";

static const char* fsSaturate = R"(
    #version 330 core

    uniform sampler2D mainTex;
	uniform float leftSeparator;
	uniform float rightSeparator;

    in vec2 uv0;
	out vec4 fragColor;

    void main()
	{
		vec4 color = texture(mainTex, uv0);
		if (uv0.x >= leftSeparator && uv0.x <= rightSeparator)
			color *= 2;
		fragColor = color;
	}
)";

static const char* fsVerticalBlur = R"(
    #version 330 core

    uniform sampler2D mainTex;
	uniform float leftSeparator;
	uniform float rightSeparator;

    uniform float offset[5] = float[](0.0, 0.002, 0.004, 0.006, 0.008);
	uniform float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

    in vec2 uv0;
	out vec4 fragColor;

    void main()
	{
		if (uv0.x >= leftSeparator && uv0.x <= rightSeparator)
		{
			fragColor = texture(mainTex, uv0) * weight[0];
			for (int i = 1; i < 5; i++)
			{
				fragColor += texture(mainTex, uv0 + vec2(0, offset[i])) * weight[i];
				fragColor += texture(mainTex, uv0 - vec2(0, offset[i])) * weight[i];
			}
		}
		else
			fragColor = texture(mainTex, uv0);
	}
)";

static const char* fsHorizontalBlur = R"(
    #version 330 core

    uniform sampler2D mainTex;
	uniform float leftSeparator;
	uniform float rightSeparator;

    uniform float offset[5] = float[](0.0, 0.002, 0.004, 0.006, 0.008);
	uniform float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

    in vec2 uv0;
	out vec4 fragColor;

    void main()
	{
		if (uv0.x >= leftSeparator && uv0.x <= rightSeparator)
		{
			fragColor = texture(mainTex, uv0) * weight[0];
			for (int i = 1; i < 5; i++)
			{
				fragColor += texture(mainTex, uv0 + vec2(offset[i], 0)) * weight[i];
				fragColor += texture(mainTex, uv0 - vec2(offset[i], 0)) * weight[i];
			}
		}
		else
			fragColor = texture(mainTex, uv0);
	}
)";