/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

namespace solo
{
    namespace gl
    {
        struct PrefabShaders
        {
            struct Vertex
            {
                static const char *skybox;
                static const char *simple;
            };

            struct Fragment
            {
                static const char *skybox;
                static const char *font;
            };
        };
    }
}

#endif
