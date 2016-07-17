#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

namespace solo
{
    struct OpenGLPrefabShaders
    {
        struct Vertex
        {
            static const char* skybox;
            static const char* simple;
        };

        struct Fragment
        {
            static const char* skybox;
            static const char* font;
        };
    };
}

#else
#   error OpenGL renderer is not supported on this platform
#endif
