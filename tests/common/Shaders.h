static struct
{
    struct
    {
        const char* basic = R"(
            #version 330 core

			in vec4 position;
			in vec2 texCoord0;

			uniform mat4 worldViewProjMatrix;
			out vec2 uv0;

			void main()
			{
				gl_Position = worldViewProjMatrix * position;
				uv0 = texCoord0;
			}
        )";

        const char* passThrough = R"(
            #version 330 core

			in vec4 position;
			in vec2 texCoord0;

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
        const char* texture = R"(
            #version 330 core

			uniform sampler2D mainTex;

			in vec2 uv0;
			out vec4 fragColor;

			void main()
			{
				fragColor = texture(mainTex, uv0);
			}
        )";

        const char* color = R"(
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