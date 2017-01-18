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

#include "SoloOpenGLPrefabShaders.h"

#ifdef SL_OPENGL_RENDERER

using namespace solo;


const char *OpenGLPrefabShaders::Vertex::skybox = R"(
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
)";


const char* OpenGLPrefabShaders::Vertex::simple = R"(
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


const char* OpenGLPrefabShaders::Fragment::skybox = R"(
    #version 330 core

    uniform samplerCube mainTex;

    smooth in vec3 eyeDir;
    out vec4 fragColor;

    void main()
    {
        fragColor = texture(mainTex, eyeDir);
    }
)";


const char* OpenGLPrefabShaders::Fragment::font = R"(
    #version 330 core

    uniform sampler2D mainTex;

    in vec2 uv0;
    out vec4 fragColor;

    void main()
    {
        vec4 c = texture(mainTex, uv0);
        fragColor = vec4(c.r, c.r, c.r, c.r);
    }
)";

#endif
