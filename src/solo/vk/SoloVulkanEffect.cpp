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
#include <shaderc/shaderc.hpp>

using namespace solo;

static auto createShaderModule(VkDevice device, const void *data, u32 size) -> VulkanResource<VkShaderModule>
{
    VkShaderModuleCreateInfo shaderModuleInfo{};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.pNext = nullptr;
    shaderModuleInfo.flags = 0;
    shaderModuleInfo.codeSize = size;
    shaderModuleInfo.pCode = reinterpret_cast<const u32*>(data);

    VulkanResource<VkShaderModule> module{device, vkDestroyShaderModule};
    SL_VK_CHECK_RESULT(vkCreateShaderModule(device, &shaderModuleInfo, nullptr, module.cleanRef()));

    return module;
}

static auto compileToSpiv(const void *src, u32 srcLen, const str &fileName, bool vertex) -> shaderc::SpvCompilationResult
{
    shaderc::Compiler compiler{};
    const shaderc::CompileOptions options{};
    auto result = compiler.CompileGlslToSpv(
        static_cast<const s8*>(src),
        srcLen,
        vertex ? shaderc_glsl_vertex_shader : shaderc_glsl_fragment_shader,
        fileName.c_str(),
        options
    );

    const auto compilationStatus = result.GetCompilationStatus();
    const auto errorMessage = result.GetErrorMessage();
    panicIf(compilationStatus != shaderc_compilation_status_success, errorMessage);

    return result;
}

auto VulkanEffect::createFromSources(Device *device,
    const void *vsSrc, u32 vsSrcLen, const str &vsFileName,
    const void *fsSrc, u32 fsSrcLen, const str &fsFileName)
    -> sptr<VulkanEffect>
{
    auto vsCompilationResult = compileToSpiv(vsSrc, vsSrcLen, vsFileName, true);
    auto fsCompilationResult = compileToSpiv(fsSrc, fsSrcLen, fsFileName, false);
    auto vsSize = (vsCompilationResult.end() - vsCompilationResult.begin()) * sizeof(u32);
    auto fsSize = (fsCompilationResult.end() - fsCompilationResult.begin()) * sizeof(u32);
    return std::make_shared<VulkanEffect>(
        device,
        vsCompilationResult.begin(), vsSize,
        fsCompilationResult.begin(), fsSize
    );
}

VulkanEffect::VulkanEffect(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen)
{
    renderer = dynamic_cast<VulkanRenderer*>(device->getRenderer());
    vertexShader = createShaderModule(renderer->getDevice(), vsSrc, vsSrcLen);
    fragmentShader = createShaderModule(renderer->getDevice(), fsSrc, fsSrcLen);
    introspectShader(static_cast<const u32*>(vsSrc), vsSrcLen / sizeof(u32), true);
    introspectShader(static_cast<const u32*>(fsSrc), fsSrcLen / sizeof(u32), false);
}

auto VulkanEffect::getUniformBuffer(const str &name) -> UniformBuffer
{
    if (uniformBuffers.count(name))
        return uniformBuffers.at(name);
    return panic<UniformBuffer>(SL_FMT("Uniform buffer ", name, " not found"));
}

auto VulkanEffect::getSampler(const str &name) -> Sampler
{
    if (samplers.count(name))
        return samplers.at(name);
    return panic<Sampler>(SL_FMT("Sampler ", name, " not found"));
}

void VulkanEffect::introspectShader(const u32 *src, u32 len, bool vertex)
{
    spirv_cross::CompilerGLSL compiler{src, len};
    const auto resources = compiler.get_shader_resources();

    for (auto &buffer: resources.uniform_buffers)
    {
        const auto& name = compiler.get_name(buffer.id);
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

    if (vertex)
    {
        for (auto &stageInput : resources.stage_inputs)
        {
            const auto name = stageInput.name;
            const auto location = compiler.get_decoration(stageInput.id, spv::DecorationLocation);
            vertexAttributes[name].location = location;
        }
    }
}

#endif
