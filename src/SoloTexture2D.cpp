#include "SoloTexture2D.h"

using namespace solo;


void Texture2D::setData(ColorFormat format, const std::vector<uint8_t>& data, int width, int height)
{
    applyData(format, data, width, height);
    size = { static_cast<float>(width), static_cast<float>(height) };
}

