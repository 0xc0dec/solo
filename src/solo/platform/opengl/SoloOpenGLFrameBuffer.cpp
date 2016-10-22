#include "SoloOpenGLFrameBuffer.h"
#include "SoloDevice.h"
#include "SoloRectTexture.h"
#include "SoloOpenGLRectTexture.h"

using namespace solo;


OpenGLFrameBuffer::OpenGLFrameBuffer(Device* device)
{
    renderer = dynamic_cast<OpenGLRenderer*>(device->getRenderer());
    handle = renderer->createFrameBuffer();
}


OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    renderer->destroyFrameBuffer(handle);
}


void OpenGLFrameBuffer::bind()
{
    renderer->setFrameBuffer(handle);
}


void OpenGLFrameBuffer::unbind()
{
    renderer->setFrameBuffer(EmptyFrameBufferHandle);
}


void OpenGLFrameBuffer::setAttachments(const std::vector<sptr<RectTexture>>& attachments)
{
    std::vector<TextureHandle> handles;
    handles.reserve(attachments.size());

    for (const auto& tex : attachments)
        handles.push_back(std::dynamic_pointer_cast<OpenGLRectTexture>(tex)->getHandle());

    renderer->updateFrameBuffer(handle, handles);
    
    if (!attachments.empty())
        size = (*attachments.begin())->getSize();
    else
        size = Vector2();
}
