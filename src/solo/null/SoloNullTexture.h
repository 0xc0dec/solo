/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTexture.h"

namespace solo
{
    namespace null
    {
        class Texture2d final: public solo::Texture2d
        {
        public:
            Texture2d(uint32_t width, uint32_t height, TextureFormat format): solo::Texture2d(width, height, format) {}

            void generateMipmaps() override final {}
            void setData(const void *data) override final {}
        };

        class CubeTexture final : public solo::CubeTexture
        {
        public:
            CubeTexture(uint32_t dimension, TextureFormat format):
                solo::CubeTexture(dimension, format)
            {
            }

            void generateMipmaps() override final {}

        protected:
            void setData(CubeTextureData *data) override final {}
        };
    }
}