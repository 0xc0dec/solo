#include "SoloOpenGLRectTexture.h"
#include "SoloOpenGLRenderer.h"
#include "SoloDevice.h"

using namespace solo;


OpenGLRectTexture::OpenGLRectTexture(Device* device)
{
    renderer = dynamic_cast<OpenGLRenderer*>(device->getRenderer());
    handle = renderer->createTexture();
}


void OpenGLRectTexture::bind()
{
    renderer->set2DTexture(handle, flags, anisotropy);
}


void OpenGLRectTexture::generateMipmaps()
{
    renderer->generateRectTextureMipmaps(handle);
}


void OpenGLRectTexture::setData(TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height)
{
    renderer->update2DTexture(handle, format, width, height, data);
    size = { static_cast<float>(width), static_cast<float>(height) };
}
