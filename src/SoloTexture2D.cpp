#include "SoloTexture2D.h"

using namespace solo;


Texture2D::Texture2D(Renderer* renderer):
    Texture(renderer)
{
}


void Texture2D::bind()
{
    renderer->set2DTexture(handle, flags, anisotropy);
}


void Texture2D::generateMipmaps()
{
    renderer->generateTexture2DMipmaps(handle);
}


void Texture2D::setData(ColorFormat format, const std::vector<uint8_t>& data, uint32_t width, uint32_t height)
{
    renderer->update2DTexture(handle, format, width, height, data.data());
    size = { static_cast<float>(width), static_cast<float>(height) };
}
