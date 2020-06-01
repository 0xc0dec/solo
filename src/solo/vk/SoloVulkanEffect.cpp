/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.codeSize = size;
    info.pCode = static_cast<const u32*>(data);

    VulkanResource<VkShaderModule> module{device, vkDestroyShaderModule};
    SL_VK_CHECK_RESULT(vkCreateShaderModule(device, &info, nullptr, module.cleanRef()));

    return module;
}

static auto compileToSpv(const void *src, u32 srcLen, const str &fileName, bool vertex) -> shaderc::SpvCompilationResult
{
	const shaderc::Compiler compiler{};
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
    asrt(compilationStatus == shaderc_compilation_status_success, "Unable to compile effect to SPV: ", errorMessage);

    return result;
}

auto VulkanEffect::fromSources(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen)
    -> sptr<VulkanEffect>
{
    const auto vsCompilationResult = compileToSpv(vsSrc, vsSrcLen, "<memory>", true);
    const auto fsCompilationResult = compileToSpv(fsSrc, fsSrcLen, "<memory>", false);
    const auto vsSize = (vsCompilationResult.end() - vsCompilationResult.begin()) * sizeof(u32);
    const auto fsSize = (fsCompilationResult.end() - fsCompilationResult.begin()) * sizeof(u32);
    return std::make_shared<VulkanEffect>(
        device,
        vsCompilationResult.begin(), static_cast<u32>(vsSize),
        fsCompilationResult.begin(), static_cast<u32>(fsSize)
    );
}

VulkanEffect::VulkanEffect(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen)
{
    renderer_ = dynamic_cast<VulkanRenderer*>(device->renderer());
    vs_ = createShaderModule(renderer_->device(), vsSrc, vsSrcLen);
    fs_ = createShaderModule(renderer_->device(), fsSrc, fsSrcLen);
    introspectShader(static_cast<const u32*>(vsSrc), vsSrcLen / sizeof(u32), true);
    introspectShader(static_cast<const u32*>(fsSrc), fsSrcLen / sizeof(u32), false);
}

void VulkanEffect::introspectShader(const u32 *src, u32 len, bool vertex)
{
	const spirv_cross::CompilerGLSL compiler{src, len};
    const auto resources = compiler.get_shader_resources();

    for (auto &buffer: resources.uniform_buffers)
    {
        const auto& name = compiler.get_name(buffer.id);
        uniformBuffers_[name].binding = compiler.get_decoration(buffer.id, spv::DecorationBinding);

        u32 size = 0;
        const auto ranges = compiler.get_active_buffer_ranges(buffer.id);
        for (auto &range: ranges)
        {
            auto memberName = compiler.get_member_name(buffer.base_type_id, range.index);
            if (memberName.empty())
                memberName = compiler.get_member_qualified_name(buffer.base_type_id, range.index);
            uniformBuffers_[name].members[memberName].size = range.range;
            uniformBuffers_[name].members[memberName].offset = range.offset;
            size += range.range;
        }

        uniformBuffers_[name].size = size;
    }

    for (auto &sampler: resources.sampled_images)
    {
        const auto binding = compiler.get_decoration(sampler.id, spv::DecorationBinding);
        samplers_[sampler.name].binding = binding;
    }

    if (vertex)
    {
        for (auto &stageInput : resources.stage_inputs)
        {
            const auto name = stageInput.name;
            const auto location = compiler.get_decoration(stageInput.id, spv::DecorationLocation);
            vertexAttributes_[name].location = location;
        }
    }
}

#endif
