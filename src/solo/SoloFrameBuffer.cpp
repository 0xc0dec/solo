/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFrameBuffer.h"
#include "SoloDevice.h"
#include "SoloTexture.h"
#include "gl/SoloOpenGLFrameBuffer.h"
#include "vk/SoloVulkanFrameBuffer.h"
#include "null/SoloNullFrameBuffer.h"

using namespace solo;

auto FrameBuffer::create(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<FrameBuffer>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return OpenGLFrameBuffer::create(attachments);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanFrameBuffer::create(device, attachments);
#endif
        default:
            return std::make_shared<NullFrameBuffer>();
    }
}

void FrameBuffer::validateNewAttachments(const vec<sptr<Texture2D>> &attachments)
{
    SL_DEBUG_PANIC(attachments.empty(), "Frame buffer must have at least one attachment");

    auto width = (std::numeric_limits<u32>::max)(), height = (std::numeric_limits<u32>::max)();
    auto depthAttachmentCount = 0;
    for (const auto &attachment : attachments)
    {
        if (attachment->getFormat() == TextureFormat::Depth24)
            SL_DEBUG_PANIC(++depthAttachmentCount > 1, "Frame buffer can only have one depth attachment");

        const auto size = attachment->getDimensions();
        if (width == (std::numeric_limits<u32>::max)())
        {
            width = static_cast<u32>(size.x());
            height = static_cast<u32>(size.y());
        }
        else
        {
            SL_DEBUG_PANIC(static_cast<u32>(size.x()) != width || static_cast<u32>(size.y()) != height,
                "Frame buffer attachments must have same dimentions");
        }
    }
}
