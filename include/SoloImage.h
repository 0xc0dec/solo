#pragma once

#include "SoloBase.h"

namespace solo
{
    enum class ColorFormat
    {
        RGB,
        RGBA
    };

    struct Image
    {
        int width;
        int height;
        ColorFormat colorFormat;
        std::vector<uint8_t> data;
    };
}