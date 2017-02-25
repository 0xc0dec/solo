/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGLCubeTexture.h"
#include "SoloDevice.h"

using namespace solo;


gl::CubeTexture::CubeTexture(Device *device)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    handle = renderer->createTexture();
}


gl::CubeTexture::~CubeTexture()
{
    renderer->destroyTexture(handle);
}


void gl::CubeTexture::bind()
{
    renderer->setCubeTexture(handle, flags, anisotropy);
}


void gl::CubeTexture::generateMipmaps()
{
    renderer->generateCubeTextureMipmaps(handle);
}


void gl::CubeTexture::setData(CubeTextureFace face, TextureFormat format, const void *data, uint32_t width, uint32_t height)
{
    renderer->updateCubeTexture(handle, face, format, width, height, data);
}

#endif
