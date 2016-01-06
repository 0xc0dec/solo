#pragma once

namespace solo
{
    struct OpenGLBuiltInShaders
    {
        static constexpr char *vsSkybox = R"s(
			#version 330 core

			layout (location = 0) in vec4 position;

			uniform mat4 projMatrix;
			uniform mat4 worldViewMatrix;
			smooth out vec3 eyeDir;

			void main()
			{
				mat4 invProjMatrix = inverse(projMatrix);
				mat3 invModelViewMatrix = inverse(mat3(worldViewMatrix));
				vec3 unprojected = (invProjMatrix * position).xyz;
				eyeDir = invModelViewMatrix * unprojected;
				gl_Position = position;
			}
		)s";

        static constexpr char *fsSkybox = R"s(
			#version 330 core

			uniform samplerCube mainTex;

			smooth in vec3 eyeDir;
			out vec4 fragColor;

			void main()
			{
				fragColor = texture(mainTex, eyeDir);
			}
		)s";
    };
}