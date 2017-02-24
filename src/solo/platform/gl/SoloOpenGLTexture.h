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
        class Texture
        {
        public:
            virtual ~Texture() {}

            virtual void bind() = 0;
        };
    }
}

#endif