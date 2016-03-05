#include "SoloTexture2D.h"

using namespace solo;


Texture2D::Texture2D(Renderer* renderer):
    Texture(renderer)
{
}


void Texture2D::apply()
{
    renderer->set2DTexture(handle, flags, anisotropy);
}


void Texture2D::setData(ColorFormat format, const std::vector<uint8_t>& data, int width, int height)
{
    renderer->update2DTexture(handle, format, width, height, data);
    size = { static_cast<float>(width), static_cast<float>(height) };
}
