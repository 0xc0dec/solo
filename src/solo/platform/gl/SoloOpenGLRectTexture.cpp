/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLRectTexture.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"

using namespace solo;


gl::RectTexture::RectTexture(Device *device)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    handle = renderer->createTexture();
}


void gl::RectTexture::bind()
{
    renderer->setRectTexture(handle, flags, anisotropy);
}


void gl::RectTexture::generateMipmaps()
{
    renderer->generateRectTextureMipmaps(handle);
}


void gl::RectTexture::setData(TextureFormat format, const uint8_t *data, uint32_t width, uint32_t height)
{
    renderer->updateRectTexture(handle, format, width, height, data);
    size = {static_cast<float>(width), static_cast<float>(height)};
}

#endif
