/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloCubeTexture.h"
#include "SoloOpenGLTexture.h"

namespace solo
{
    namespace gl
    {
        class CubeTexture final : public solo::CubeTexture, public Texture
        {
        public:
            void bind() override final;
            void generateMipmaps() override final;

            void setData(CubeTextureFace face, ImageFormat format, const void *data, uint32_t width, uint32_t height) override final;
        };
    }
}

#endif
