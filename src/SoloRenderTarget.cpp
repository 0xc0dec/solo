#include "SoloRenderTarget.h"
#include "SoloRenderer.h"
#include "SoloTexture2D.h"

using namespace solo;


RenderTarget::RenderTarget(Renderer* renderer):
    renderer(renderer)
{
    handle = renderer->createFrameBuffer();
}


RenderTarget::~RenderTarget()
{
    renderer->destroyFrameBuffer(handle);
}


void RenderTarget::bind()
{
    renderer->setFrameBuffer(handle);
}


void RenderTarget::unbind()
{
    renderer->setFrameBuffer(EmptyFrameBufferHandle);
}


void RenderTarget::setAttachments(const std::vector<shared<Texture2D>> attachments)
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
