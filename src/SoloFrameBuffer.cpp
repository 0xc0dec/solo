#include "SoloFrameBuffer.h"
#include "SoloRenderer.h"
#include "SoloTexture2D.h"

using namespace solo;


FrameBuffer::FrameBuffer(Renderer* renderer):
    renderer(renderer)
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


void FrameBuffer::setAttachments(const std::vector<shared<Texture2D>> attachments)
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
