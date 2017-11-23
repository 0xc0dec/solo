/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFrameBuffer.h"
#include "SoloDevice.h"
#include "gl/SoloOpenGLFrameBuffer.h"
#include "null/SoloNullFrameBuffer.h"

using namespace solo;

auto FrameBuffer::create(Device *device) -> sptr<FrameBuffer>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::OpenGLFrameBuffer>();
#endif
        default:
            return std::make_shared<null::FrameBuffer>();
    }
}