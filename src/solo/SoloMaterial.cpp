/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMaterial.h"
#include "SoloTexture.h"
#include "SoloDevice.h"
#include "platform/gl/SoloOpenGLMaterial.h"
#include "platform/vk/SoloVulkanMaterial.h"
#include "platform/null/SoloNullMaterial.h"

using namespace solo;

auto Material::create(Device *device, sptr<Effect> effect) -> sptr<Material>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Material>(effect);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<vk::Material>(effect);
#endif
        default:
            return std::make_shared<null::Material>(effect);
    }
}

Material::Material(sptr<Effect> effect):
    effect(effect)
{
}
