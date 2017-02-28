/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include <GL/glew.h>

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

            auto getHandle() const -> GLuint;

            virtual void bind() = 0;

        protected:
            GLuint handle = 0;
        };

        inline auto Texture::getHandle() const -> GLuint
        {
            return handle;
        }
    }
}

#endif