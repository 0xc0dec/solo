/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloRenderer.h"


namespace solo
{
    struct Image
    {
        uint32_t width;
        uint32_t height;
        TextureFormat format;
        std::vector<uint8_t> data;
    };
}