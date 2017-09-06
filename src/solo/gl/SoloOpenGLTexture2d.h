/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloTexture2d.h"
#include "SoloOpenGLTexture.h"

namespace solo
{
    class Device;

    namespace gl
    {
        class Texture2d final: public solo::Texture2d, public Texture
        {
        public:
            Texture2d(uint32_t width, uint32_t height, TextureFormat format);

            void setData(const void *data) override final;
            void bind() override final;
            void generateMipmaps() override final;
        };
    }
}

#endif
