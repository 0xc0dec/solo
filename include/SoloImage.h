#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"


namespace solo
{
    struct Image
    {
        uint32_t width;
        uint32_t height;
        TextureFormat colorFormat;
        std::vector<uint8_t> data;
    };
}