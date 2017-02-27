/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLFrameBuffer.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloRectTexture.h"
#include "SoloOpenGLRectTexture.h"

using namespace solo;


gl::FrameBuffer::FrameBuffer(Device *device)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    handle = renderer->createFrameBuffer();
}


gl::FrameBuffer::~FrameBuffer()
{
    renderer->destroyFrameBuffer(handle);
}


void gl::FrameBuffer::bind() const
{
    renderer->setFrameBuffer(handle);
}


void gl::FrameBuffer::unbind() const
{
    renderer->setFrameBuffer(EmptyHandle);
}


void gl::FrameBuffer::setAttachments(const std::vector<sptr<solo::RectTexture>> &attachments)
{
    std::vector<uint32_t> handles;
    handles.reserve(attachments.size());

    for (const auto &tex : attachments)
        handles.push_back(std::dynamic_pointer_cast<RectTexture>(tex)->getHandle());

    renderer->updateFrameBuffer(handle, handles);

    if (!attachments.empty())
        size = (*attachments.begin())->getSize();
    else
        size = {};
}

#endif
