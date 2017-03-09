/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    enum class TextureFormat;

    struct Image
    {
        uint32_t width;
        uint32_t height;
        TextureFormat format;
        std::vector<uint8_t> data;
    };
}