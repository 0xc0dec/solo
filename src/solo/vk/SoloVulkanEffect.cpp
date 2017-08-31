/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanEffect.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkan.h"
#include "SoloVulkanRenderer.h"
#include "SoloFileSystem.h"

using namespace solo;
// TODO remove "using namespace vk" from other places

auto vk::Effect::create(Device *device, EffectPrefab prefab) -> sptr<Effect>
{
    // TODO remove hackery!
    return std::make_shared<Effect>(device, "", "");
    return nullptr;
}

vk::Effect::Effect(Device *device, const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen)
{

}

vk::Effect::Effect(Device *device, const std::string &vsSrc, const std::string &fsSrc)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    auto vsBytes = device->getFileSystem()->readBytes("../../assets/triangle.vert.spv");
    auto fsBytes = device->getFileSystem()->readBytes("../../assets/triangle.frag.spv");
    vertexShader = createShader(renderer->getDevice(), vsBytes.data(), vsBytes.size());
    fragmentShader = createShader(renderer->getDevice(), fsBytes.data(), fsBytes.size());
}

vk::Effect::~Effect()
{
}

#endif
