/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMaterial.h"
#include "SoloTexture.h"
#include "SoloDevice.h"
#include "gl/SoloOpenGLMaterial.h"
#include "vk/SoloVulkanMaterial.h"
#include "null/SoloNullMaterial.h"

using namespace solo;

auto Material::create(Device *device, sptr<Effect> effect) -> sptr<Material>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLMaterial>(effect);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanMaterial>(effect);
#endif
        default:
            return std::make_shared<NullMaterial>();
    }
}

auto Material::createFromPrefab(Device *device, MaterialPrefab prefab) -> sptr<Material>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return OpenGLMaterial::createFromPrefab(device, prefab);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanMaterial::createFromPrefab(device, prefab);
#endif
        default:
            return std::make_shared<NullMaterial>();
    }
}

void Material::setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor)
{
    srcBlendFactor = srcFactor;
    dstBlendFactor = dstFactor;
}
