/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloRectTexture.h"

namespace solo
{
    namespace null
    {
        class RectTexture final: public solo::RectTexture
        {
        public:
            RectTexture(uint32_t width, uint32_t height, TextureFormat format): solo::RectTexture(width, height, format) {}

            void generateMipmaps() override final {}
            void setData(const void *data) override final {}
        };
    }
}