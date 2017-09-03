/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanEffect.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkan.h"
#include "SoloVulkanRenderer.h"

using namespace solo;
// TODO remove "using namespace vk" from other places

auto vk::Effect::createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>
{
    // TODO
    return nullptr;
}

vk::Effect::Effect(Device *device, const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    vertexShader = createShader(renderer->getDevice(), vsSrc, vsSrcLen);
    fragmentShader = createShader(renderer->getDevice(), fsSrc, fsSrcLen);
}

vk::Effect::~Effect()
{
}

#endif
