/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanEffect.h"


#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkan.h"
#include "SoloVulkanRenderer.h"
#include <spirv_cross/spirv.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

using namespace solo;
// TODO remove "using namespace vk" from other places

auto vk::Effect::createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>
{
    // TODO
    return nullptr;
}

vk::Effect::Effect(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    vertexShader = createShader(renderer->getDevice(), vsSrc, vsSrcLen);
    fragmentShader = createShader(renderer->getDevice(), fsSrc, fsSrcLen);
    introspectShader(static_cast<const u32*>(vsSrc), vsSrcLen / sizeof(u32));
    introspectShader(static_cast<const u32*>(fsSrc), fsSrcLen / sizeof(u32));
}

vk::Effect::~Effect()
{
}

auto vk::Effect::getUniformBuffer(const str &name) -> UniformBuffer
{
    if (uniformBuffers.count(name))
        return uniformBuffers.at(name);
    SL_PANIC(SL_FMT("Uniform buffer ", name, " not found"));
    return UniformBuffer{};
}

auto vk::Effect::getSampler(const str &name) -> Sampler
{
    if (samplers.count(name))
        return samplers.at(name);
    SL_PANIC(SL_FMT("Sampler ", name, " not found"));
    return Sampler{};
}

void vk::Effect::introspectShader(const u32 *src, u32 len)
{
    spirv_cross::CompilerGLSL compiler{src, len};
    const auto resources = compiler.get_shader_resources();

    for (auto &buffer: resources.uniform_buffers)
    {
        const auto name = compiler.get_name(buffer.id);
        uniformBuffers[name].binding = compiler.get_decoration(buffer.id, spv::DecorationBinding);

        u32 size = 0;
        const auto ranges = compiler.get_active_buffer_ranges(buffer.id);
        for (auto &range: ranges)
        {
            auto memberName = compiler.get_member_name(buffer.base_type_id, range.index);
            if (memberName.empty())
                memberName = compiler.get_member_qualified_name(buffer.base_type_id, range.index);
            uniformBuffers[name].members[memberName].size = range.range;
            uniformBuffers[name].members[memberName].offset = range.offset;
            size += range.range;
        }

        uniformBuffers[name].size = size;
    }

    for (auto &sampler: resources.sampled_images)
    {
        const auto binding = compiler.get_decoration(sampler.id, spv::DecorationBinding);
        samplers[sampler.name].binding = binding;
    }
}

#endif
