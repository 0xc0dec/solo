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
            CubeTexture(uint32_t width, uint32_t height, uint32_t depth, TextureFormat format);

            void bind() override final;
            void generateMipmaps() override final;

        protected:
            void setData(CubeTextureData *data) override final;
        };
    }
}

#endif
