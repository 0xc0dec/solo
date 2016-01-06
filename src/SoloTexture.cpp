#include "SoloTexture.h"
#include "SoloDevice.h"
#include "platform/stub/SoloStubTexture2D.h"
#include "platform/stub/SoloStubCubeTexture.h"
#include "platform/opengl/SoloOpenGLTexture2D.h"
#include "platform/opengl/SoloOpenGLCubeTexture.h"

using namespace solo;


shared<Texture2D> Texture::create2D(DeviceMode mode)
{
    if (mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(OpenGLTexture2D);
    return SL_NEW_SHARED(StubTexture2D);
}


shared<CubeTexture> Texture::createCube(DeviceMode mode)
{
    if (mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(OpenGLCubeTexture);
    return SL_NEW_SHARED(StubCubeTexture);
}


void Texture::setWrapping(TextureWrapping wrap)
{
    horizontalWrapping = wrap;
    verticalWrapping = wrap;
}