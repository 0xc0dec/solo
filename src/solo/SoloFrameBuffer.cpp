/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloFrameBuffer.h"
#include "SoloRenderer.h"
#include "SoloRectTexture.h"
#include "SoloDevice.h"

using namespace solo;


auto FrameBuffer::create() -> sptr<FrameBuffer>
{
    return std::shared_ptr<FrameBuffer>(new FrameBuffer(Device::get()));
}


FrameBuffer::FrameBuffer(Device* device):
    renderer(device->getRenderer())
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


void FrameBuffer::setAttachments(const std::vector<sptr<RectTexture>>& attachments)
{
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
