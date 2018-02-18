/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFrameBuffer.h"
#include "SoloDevice.h"
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