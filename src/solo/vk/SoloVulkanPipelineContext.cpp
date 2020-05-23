/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanPipelineContext.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanMaterial.h"
#include "SoloVulkanDevice.h"
#include "SoloVulkanMesh.h"
#include "SoloVulkanRenderPass.h"
#include "SoloVulkanTexture.h"
#include "SoloCamera.h"
#include "SoloTransform.h"

using namespace solo;

VulkanPipelineContext::VulkanPipelineContext(VulkanDevice *device, size_t key):
	device_(device), key_(key)
{
}

VulkanPipelineContext::VulkanPipelineContext(VulkanPipelineContext &&other) noexcept:
	device_(other.device_),
	uniformBuffers_(std::move(other.uniformBuffers_)),
	pipeline_(std::move(other.pipeline_)),
	descSet_(std::move(other.descSet_)),
	lastMaterialStateHash_(other.lastMaterialStateHash_),
	lastMeshLayoutHash_(other.lastMeshLayoutHash_),
	key_(other.key_),
	frameOfLastUse_(other.frameOfLastUse_)
{
}

void VulkanPipelineContext::update(VulkanMaterial *material, VulkanMesh *mesh, VulkanRenderPass *renderPass,
	Camera *camera, Transform *transform)
{
	const auto effect = dynamic_cast<VulkanEffect*>(material->effect().get());

	if (!descSet_)
    {
        const auto &uniformBufs = effect->uniformBuffers();
        const auto &effectSamplers = effect->samplers();

        VulkanDescriptorSetConfig cfg;

        for (const auto &pair : uniformBufs)
        {
            const auto bufferName = pair.first;
            uniformBuffers_[bufferName] = VulkanBuffer::uniformHostVisible(*device_, pair.second.size);
            cfg.addUniformBuffer(pair.second.binding);
        }

        for (auto &pair : effectSamplers)
            cfg.addSampler(pair.second.binding);

        descSet_ = VulkanDescriptorSet(*device_, cfg);
    }

	const auto materialStateHash = material->stateHash();
    const auto meshLayoutHash = mesh->layoutHash();

    if (!pipeline_ || materialStateHash != lastMaterialStateHash_ || meshLayoutHash != lastMeshLayoutHash_)
    {
        auto pipelineConfig = VulkanPipelineConfig(effect->vsModule(), effect->fsModule())
            .withDescriptorSetLayout(descSet_.layout())
            .withFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
            .withColorBlendAttachmentCount(renderPass->colorAttachmentCount());
        
        material->configurePipeline(pipelineConfig);
        mesh->configurePipeline(pipelineConfig, effect);

        pipeline_ = VulkanPipeline{*device_, *renderPass, pipelineConfig};
        lastMaterialStateHash_ = materialStateHash;
        lastMeshLayoutHash_ = meshLayoutHash;
    }

	const auto &uniformBufs = effect->uniformBuffers();
	const auto &materialSamplers = material->samplers();

	// TODO Run set updater not so often - only when something really changes

    // TODO Not necessary (?), buffers don't change anyway, only their content
    for (auto &pair : uniformBuffers_)
    {
        const auto &info = uniformBufs.at(pair.first);
        auto &buffer = pair.second;
        descSet_.updateUniformBuffer(info.binding, buffer, 0, info.size); // TODO use single large buffer?
    }

    for (auto &pair : materialSamplers)
    {
        auto &info = pair.second;
        descSet_.updateSampler(
            info.binding,
            info.texture->image().view(),
            info.texture->sampler(),
            info.texture->image().layout());
    }

    auto &bufferItems = material->bufferItems();
    for (auto &p : bufferItems)
    {
        auto &buffer = uniformBuffers_[p.first];
        for (auto &pp : p.second)
            pp.second.write(buffer, camera, transform);
    }
}

#endif
