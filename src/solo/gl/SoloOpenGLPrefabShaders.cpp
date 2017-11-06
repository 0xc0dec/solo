/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLPrefabShaders.h"

#ifdef SL_OPENGL_RENDERER

using namespace solo;

const s8 *gl::PrefabShaders::Vertex::skybox = R"(
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

const s8* gl::PrefabShaders::Vertex::positionAndTexCoord = R"(
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

const s8* gl::PrefabShaders::Fragment::skybox = R"(
    #version 330 core

    uniform samplerCube mainTex;

    smooth in vec3 eyeDir;
    out vec4 fragColor;

    void main()
    {
        fragColor = texture(mainTex, eyeDir);
    }
)";

const s8* gl::PrefabShaders::Fragment::font = R"(
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
