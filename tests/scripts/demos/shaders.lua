return
{
	vertex = {
		basic = [[
			#version 330 core

			in vec4 position;
			in vec3 normal;
			in vec2 texCoord0;

			uniform mat4 worldViewProjMatrix;
			out vec2 uv0;
			out vec3 n;

			void main()
			{
				gl_Position = worldViewProjMatrix * position;
				uv0 = texCoord0;
			}
		]],

		passThrough = [[
			#version 330 core

			in vec4 position;
			in vec2 texCoord0;

			out vec2 uv0;

			void main()
			{
				gl_Position = position;
				uv0 = texCoord0;
			}
		]],

		wavy = [[
			#version 330 core

			in vec4 position;
			in vec3 normal;
			in vec2 texCoord0;

			uniform mat4 worldViewProjMatrix;
			uniform float time;
			out vec2 uv0;
			out vec3 n;

			void main()
			{
				float sx = sin(position.x * 32.0 + time * 4.0) * 0.5 + 0.5;
				float cy = cos(position.y * 32.0 + time * 4.0) * 0.5 + 0.5;
				vec3 displacement = vec3(sx, cy, sx * cy);
				vec3 n = normal.xyz * 2.0 - 1.0;
				vec4 pos = vec4(position.xyz + n * displacement * 0.03, 1);
				gl_Position = worldViewProjMatrix * pos;
				uv0 = texCoord0;
			}
		]],

		basicLighting = [[
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
		]]
	},

	fragment = {
		color = [[
			#version 330 core

			uniform vec4 color;

			in vec2 uv0;
			out vec4 fragColor;

			void main()
			{
				fragColor = color;
			}
		]],

		texture =[[
			#version 330 core

			uniform sampler2D mainTex;

			in vec2 uv0;
			out vec4 fragColor;

			void main()
			{
				fragColor = texture(mainTex, uv0);
			}
		]],

		checker = [[
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
		]],

		textureWithLighting = [[
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
		]],

		postProcessGrayscale = [[
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
		]],

		postProcessSaturate = [[
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
		]],

		postProcessVerticalBlur = [[
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
		]],

		postProcessHorizontalBlur = [[
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
		]]
	}
}
