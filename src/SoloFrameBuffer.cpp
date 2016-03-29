#include "SoloFrameBuffer.h"
#include "SoloRenderer.h"
#include "SoloTexture2D.h"
#include "SoloDevice.h"

using namespace solo;


sptr<FrameBuffer> FrameBuffer::create()
{
    return std::shared_ptr<FrameBuffer>(new FrameBuffer());
}


FrameBuffer::FrameBuffer():
    renderer(Device::get()->getRenderer())
{
    handle = renderer->createFrameBuffer();
}


FrameBuffer::~FrameBuffer()
{
    renderer->destroyFrameBuffer(handle);
}


void FrameBuffer::bind()
{
    renderer->setFrameBuffer(handle);
}


void FrameBuffer::unbind()
{
    renderer->setFrameBuffer(EmptyFrameBufferHandle);
}


void FrameBuffer::setAttachments(const std::vector<sptr<Texture2D>> attachments)
{
    // TODO this is quite smelly
    std::vector<TextureHandle> handles;
    handles.reserve(attachments.size());

    for (const auto& tex : attachments)
        handles.push_back(tex->getHandle());

    renderer->updateFrameBuffer(handle, handles);
    
    if (!attachments.empty())
        size = (*attachments.begin())->getSize();
    else
        size = Vector2();
}
