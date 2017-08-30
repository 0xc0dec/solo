/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloCubeTexture.h"
#include "SoloDevice.h"
#include "gl/SoloOpenGLCubeTexture.h"
#include "null/SoloNullCubeTexture.h"

using namespace solo;

auto CubeTexture::create(Device *device) -> sptr<CubeTexture>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::CubeTexture>();
#endif
        default:
            return std::make_shared<null::CubeTexture>();
    }
}

CubeTexture::CubeTexture()
{
    rebuildFlags();
}

void CubeTexture::setWrapping(TextureWrapping wrap)
{
    verticalWrapping = wrap;
    horizontalWrapping = wrap;
    depthWrapping = wrap;
    rebuildFlags();
}

void CubeTexture::rebuildFlags()
{
    Texture::rebuildFlags();

    switch (depthWrapping)
    {
        case TextureWrapping::Clamp:
            flags |= TextureFlags::DepthWrapClamp;
            break;
        case TextureWrapping::Repeat:
            flags |= TextureFlags::DepthWrapRepeat;
            break;
        default:
            break;
    }
}
