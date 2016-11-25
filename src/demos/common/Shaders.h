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

static struct
{
    struct
    {
        const char *basic = R"(
            #version 330 core

			layout (location = 0) in vec4 position;
			layout (location = 1) in vec2 texCoord0;

			uniform mat4 worldViewProjMatrix;
			out vec2 uv0;

			void main()
			{
				gl_Position = worldViewProjMatrix * position;
				uv0 = texCoord0;
			}
        )";

        const char *passThrough = R"(
            #version 330 core

			layout (location = 0) in vec4 position;
			layout (location = 1) in vec2 texCoord0;

			out vec2 uv0;

			void main()
			{
				gl_Position = position;
				uv0 = texCoord0;
			}
        )";
    } vertex;

    struct
    {
        const char *texture = R"(
            #version 330 core

			uniform sampler2D mainTex;

			in vec2 uv0;
			out vec4 fragColor;

			void main()
			{
				fragColor = texture(mainTex, uv0);
			}
        )";

        const char *color = R"(
            #version 330 core

            uniform vec4 color;

            in vec2 uv0;
	        out vec4 fragColor;

            void main()
	        {
		        fragColor = color;
	        }
        )";
    } fragment;
} commonShaders;