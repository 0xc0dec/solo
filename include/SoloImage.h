/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    enum class ImageFormat
    {
        Red,
        RGB,
        RGBA,
        Alpha
    };

    struct Image
    {
        uint32_t width;
        uint32_t height;
        ImageFormat format;
        std::vector<uint8_t> data;
    };
}