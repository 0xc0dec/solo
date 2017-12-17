/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "gl/SoloOpenGLEffect.h"
#include "vk/SoloVulkanEffect.h"
#include "null/SoloNullEffect.h"

using namespace solo;

auto Effect::loadFromFiles(Device *device, const str &vsPath, const str &fsPath) -> sptr<Effect>
{
    auto vsBytes = device->getFileSystem()->readBytes(vsPath);
    auto fsBytes = device->getFileSystem()->readBytes(fsPath);

    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLEffect>(
                vsBytes.data(), vsBytes.size(),
                fsBytes.data(), fsBytes.size()
            );
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanEffect::createFromSource(
                device,
                vsBytes.data(), vsBytes.size(), vsPath,
                fsBytes.data(), fsBytes.size(), fsPath
            );
#endif
        default:
            return std::make_shared<NullEffect>();
    }
}

auto Effect::createFromSource(
    Device *device,
    const void *vsSrc, u32 vsSrcLen,
    const void *fsSrc, u32 fsSrcLen) -> sptr<Effect>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLEffect>(vsSrc, vsSrcLen, fsSrc, fsSrcLen);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanEffect::createFromSource(device, vsSrc, vsSrcLen, "vert-src", fsSrc, fsSrcLen, "frag-src");
#endif
        default:
            return std::make_shared<NullEffect>();
    }
}
