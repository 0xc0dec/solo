/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloEffect.h"
#include "SoloDevice.h"
#include "platform/gl/SoloOpenGLEffect.h"
#include "platform/vk/SoloVulkanEffect.h"
#include "platform/null/SoloNullEffect.h"

using namespace solo;

auto Effect::create(Device *device, const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen) -> sptr<Effect>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Effect>(vsSrc, vsSrcLen, fsSrc, fsSrcLen);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<vk::Effect>(device, vsSrc, vsSrcLen, fsSrc, fsSrcLen);
#endif
        default:
            return std::make_shared<null::Effect>();
    }
}

auto Effect::createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return gl::Effect::create(prefab);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return vk::Effect::create(device, prefab);
#endif
        default:
            return std::make_shared<null::Effect>();
    }
}
