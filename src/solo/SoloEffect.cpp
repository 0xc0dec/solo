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

static auto create(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen) -> sptr<Effect>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::OpenGLEffect>(vsSrc, vsSrcLen, fsSrc, fsSrcLen);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanEffect>(device, vsSrc, vsSrcLen, fsSrc, fsSrcLen);
#endif
        default:
            return std::make_shared<null::NullEffect>();
    }
}

auto Effect::loadFromFiles(Device *device, const str &vsPath, const str &fsPath) -> sptr<Effect>
{
    auto vsBytes = device->getFileSystem()->readBytes(vsPath);
    auto fsBytes = device->getFileSystem()->readBytes(fsPath);
    return create(device,
        vsBytes.data(), static_cast<u32>(vsBytes.size()),
        fsBytes.data(), static_cast<u32>(fsBytes.size()));
}

auto Effect::createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return gl::OpenGLEffect::createFromPrefab(prefab);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanEffect::createFromPrefab(device, prefab);
#endif
        default:
            return std::make_shared<null::NullEffect>();
    }
}
