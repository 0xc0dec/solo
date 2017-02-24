/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloCubeTexture.h"
#include "SoloOpenGLTexture.h"
#include "SoloOpenGLRenderer.h"

namespace solo
{
    namespace gl
    {
        class CubeTexture final : public solo::CubeTexture, public Texture
        {
        public:
            explicit CubeTexture(Device *device);
            ~CubeTexture();

            void bind() override final;
            void generateMipmaps() override final;

            void setData(CubeTextureFace face, TextureFormat format, const uint8_t *data, uint32_t width, uint32_t height) override final;

        private:
            Renderer *renderer = nullptr;
            uint32_t handle = EmptyHandle;
        };
    }
}

#endif
