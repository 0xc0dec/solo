/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGL.h"

namespace solo
{
    namespace gl
    {
        class Texture
        {
        public:
            SL_DISABLE_COPY_AND_MOVE(Texture)

            Texture();
            virtual ~Texture();

            auto getHandle() const -> GLuint { return handle; }

            virtual void bind() = 0;

        protected:
            GLuint handle = 0;
        };
    }
}

#endif