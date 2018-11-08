/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloRenderer.h"
#include "SoloDevice.h"
#include "gl/SoloOpenGLRenderer.h"
#include "vk/SoloVulkanRenderer.h"

using namespace solo;

auto Renderer::create(Device *device, const FriendToken<Device> &) -> sptr<Renderer>
{
    switch (device->mode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLRenderer>(device);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanRenderer>(device);
#endif
        default:
            SL_DEBUG_PANIC(true, "Unknown device mode");
            break;
    }
}

void Renderer::renderFrame(std::function<void()> render)
{
    beginFrame();
    render();
    endFrame();
}
