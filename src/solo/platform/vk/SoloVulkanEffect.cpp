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


auto vk::Effect::create(Device *device, EffectPrefab prefab) -> sptr<Effect>
{
    SL_PANIC("Not implemented");
    return nullptr;
}


vk::Effect::Effect(Device *device, const std::string &vsSrc, const std::string &fsSrc)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    vertexShader = createShader(renderer->getDevice(), vsSrc.c_str(), vsSrc.size());
    fragmentShader = createShader(renderer->getDevice(), fsSrc.c_str(), fsSrc.size());
}


vk::Effect::~Effect()
{
}


void vk::Effect::apply()
{
}


#endif
