#include "SoloOpenGLCubeTexture.h"
#include "SoloDevice.h"

using namespace solo;


OpenGLCubeTexture::OpenGLCubeTexture(Device* device)
{
    renderer = dynamic_cast<OpenGLRenderer*>(device->getRenderer());
    handle = renderer->createTexture();
}


OpenGLCubeTexture::~OpenGLCubeTexture()
{
    renderer->destroyTexture(handle);
}


void OpenGLCubeTexture::bind()
{
    renderer->setCubeTexture(handle, flags, anisotropy);
}


void OpenGLCubeTexture::generateMipmaps()
{
    renderer->generateCubeTextureMipmaps(handle);
}


void OpenGLCubeTexture::setData(CubeTextureFace face, TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height)
{
    renderer->updateCubeTexture(handle, face, format, width, height, data);
}
