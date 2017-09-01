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
        class RectTexture : public solo::RectTexture
        {
        public:
            void generateMipmaps() override final {}
            void setData(ImageFormat format, const void *data, uint32_t width, uint32_t height) override final {}
        };
    }
}