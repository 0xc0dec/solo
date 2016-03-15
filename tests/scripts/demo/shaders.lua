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
			out vec3 n;

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

		postProcessHalfGrayscale = [[
			#version 330 core

			uniform sampler2D mainTex;
			uniform float separator;

			in vec2 uv0;
			out vec4 fragColor;

			void main()
			{
				vec4 color = texture(mainTex, uv0);
				if (uv0.x <= separator)
				{
					float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
					color = vec4(gray, gray, gray, 1);
				}
				fragColor = color;
			}
		]],

		postProcessHalfSaturate = [[
			#version 330 core

			uniform sampler2D mainTex;
			uniform float separator;

			in vec2 uv0;
			out vec4 fragColor;

			void main()
			{
				vec4 color = texture(mainTex, uv0);
				if (uv0.x >= separator)
					color *= 2;
				fragColor = color;
			}
		]]
	}
}
