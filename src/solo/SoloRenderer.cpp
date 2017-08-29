/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloRenderer.h"
#include "SoloDevice.h"
#include "platform/gl/SoloOpenGLRenderer.h"
#include "platform/vk/SoloVulkanRenderer.h"
#include "platform/null/SoloNullRenderer.h"

using namespace solo;

auto Renderer::create(Device *device, const FriendToken<Device> &) -> sptr<Renderer>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Renderer>(device);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<vk::Renderer>(device);
#endif
        default:
            return std::make_shared<null::Renderer>();
    }
}

void Renderer::renderFrame(std::function<void()> render)
{
    beginFrame();
    render();
    endFrame();
}
