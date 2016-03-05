#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"

namespace solo
{
    struct Image
    {
        int width;
        int height;
        ColorFormat colorFormat;
        std::vector<uint8_t> data;
    };
}