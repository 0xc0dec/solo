/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloMaterial.h"
#include "SoloTexture.h"
#include "SoloDevice.h"
#include "gl/SoloOpenGLMaterial.h"
#include "vk/SoloVulkanMaterial.h"

using namespace solo;

auto Material::fromEffect(Device *device, sptr<Effect> effect) -> sptr<Material> {
    switch (device->mode()) {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLMaterial>(effect);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanMaterial>(effect);
#endif
        default:
            panic("Unknown device mode");
            return nullptr;
    }
}

void Material::setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor) {
    srcBlendFactor_ = srcFactor;
    dstBlendFactor_ = dstFactor;
}
