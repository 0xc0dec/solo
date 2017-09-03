/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloRectTexture.h"
#include "SoloOpenGLTexture.h"

namespace solo
{
    class Device;

    namespace gl
    {
        class RectTexture final: public solo::RectTexture, public Texture
        {
        public:
            RectTexture(uint32_t width, uint32_t height, TextureFormat format);

            void setData(const void *data) override final;
            void bind() override final;
            void generateMipmaps() override final;
            
        };
    }
}

#endif
