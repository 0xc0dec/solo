/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloFrameBuffer.h"
#include "SoloDevice.h"
#include "SoloTexture.h"
#include "gl/SoloOpenGLFrameBuffer.h"
#include "vk/SoloVulkanFrameBuffer.h"

using namespace solo;

auto FrameBuffer::fromAttachments(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<FrameBuffer> {
    switch (device->mode()) {
#ifdef SL_OPENGL_RENDERER
    case DeviceMode::OpenGL:
        return OpenGLFrameBuffer::fromAttachments(attachments);
#endif
#ifdef SL_VULKAN_RENDERER
    case DeviceMode::Vulkan:
        return VulkanFrameBuffer::fromAttachments(device, attachments);
#endif
    default:
        panic("Unknown device mode");
        return nullptr;
    }
}

void FrameBuffer::validateNewAttachments(const vec<sptr<Texture2D>> &attachments) {
    panicIf(attachments.empty(), "Frame buffer must have at least one attachment");

    auto width = (std::numeric_limits<u32>::max)(), height = (std::numeric_limits<u32>::max)();
    auto depthAttachmentCount = 0;
    for (const auto &attachment : attachments) {
        if (attachment->format() == TextureFormat::Depth24)
            panicIf(++depthAttachmentCount > 1, "Frame buffer can only have one depth attachment");

        const auto size = attachment->dimensions();
        if (width == (std::numeric_limits<u32>::max)()) {
            width = static_cast<u32>(size.x());
            height = static_cast<u32>(size.y());
        } else {
            panicIf(static_cast<u32>(size.x()) != width && static_cast<u32>(size.y()) == height,
                    "Frame buffer attachments must have same dimentions");
        }
    }
}
