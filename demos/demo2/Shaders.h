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

static const char* fsStitches = R"(
    #version 330 core

    uniform sampler2D mainTex;
    uniform sampler2D stitchTex;
    uniform vec2 stitchCount;
    uniform vec2 resolution;

    in vec2 uv0;
    out vec4 fragColor;

    void main()
    {
        vec2 colorBlock = floor(uv0 * resolution);
        vec4 color = texture(mainTex, uv0);
        vec4 prevColor = texture(mainTex, vec2(uv0.x, uv0.y + 1.0 / resolution.y));

        float stitchUvX = fract(stitchCount.x * (uv0.x + pow(colorBlock.y, 2.0) / resolution.x * 2.0));
        float stitchUvY = fract(stitchCount.y * uv0.y);
        float nextStitchUvX = fract(stitchCount.x * (uv0.x + pow(colorBlock.y + 1.0, 2.0) / resolution.x * 2.0));
        if (stitchUvY > 0.5)
        {
            vec2 stitchUV = vec2(stitchUvX, stitchUvY);
            fragColor = texture(stitchTex, stitchUV) * color;
            vec2 topStitchUV = vec2(nextStitchUvX, fract(stitchUvY - 0.5));
            vec4 topStitchColor = texture(stitchTex, topStitchUV) * prevColor;
            if (topStitchColor.a > 0.05)
                fragColor = mix(fragColor, topStitchColor, 1.0 - fragColor.a);
        }
        else
        {
            vec2 stitchUV = vec2(nextStitchUvX, stitchUvY);
            fragColor = texture(stitchTex, stitchUV) * prevColor;
            vec2 bottomStitchUV = vec2(stitchUvX, fract(stitchUvY + 0.5));
            vec4 bottomStitchColor = texture(stitchTex, bottomStitchUV) * color;
            if (bottomStitchColor.a > 0.7)
                fragColor = mix(bottomStitchColor, fragColor, 1.0 - prevColor.a);
        }

        fragColor = mix(vec4(0, 0, 0, 1), fragColor, fragColor.a);
    }
)";