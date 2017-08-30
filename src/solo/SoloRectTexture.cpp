/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloRectTexture.h"
#include "SoloDevice.h"
#include "gl/SoloOpenGLRectTexture.h"
#include "null/SoloNullRectTexture.h"

using namespace solo;

sptr<RectTexture> RectTexture::create(Device *device)
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::RectTexture>();
#endif
        default:
            return std::make_shared<null::RectTexture>();
    }
}
