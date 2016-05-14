#include "SoloRectTexture.h"

using namespace solo;


sptr<RectTexture> RectTexture::create()
{
    return std::make_shared<RectTexture>();
}


void RectTexture::bind()
{
    renderer->set2DTexture(handle, flags, anisotropy);
}


void RectTexture::generateMipmaps()
{
    renderer->generateRectTextureMipmaps(handle);
}


void RectTexture::setData(TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height)
{
    renderer->update2DTexture(handle, format, width, height, data);
    size = { static_cast<float>(width), static_cast<float>(height) };
}
