/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanPipelineContext.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanMaterial.h"
#include "SoloVulkanDriverDevice.h"
#include "SoloVulkanMesh.h"
#include "SoloVulkanRenderPass.h"
#include "SoloVulkanTexture.h"
#include "SoloCamera.h"
#include "SoloTransform.h"

using namespace solo;

static auto toVertexFormat(const VertexAttribute &attr) -> VkFormat
{
    switch (attr.elementCount)
    {
        case 1: return VK_FORMAT_R32_SFLOAT;
        case 2: return VK_FORMAT_R32G32_SFLOAT;
        case 3: return VK_FORMAT_R32G32B32_SFLOAT;
        case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		default:
		    panic("Unsupported vertex attribute element count");
		    return VK_FORMAT_UNDEFINED;	
    }
}

static auto toBlendFactor(BlendFactor factor) -> VkBlendFactor
{
    switch (factor)
    {
        case BlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
        case BlendFactor::One: return VK_BLEND_FACTOR_ONE;
        case BlendFactor::SrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor: return VK_BLEND_FACTOR_DST_COLOR;
        case BlendFactor::OneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case BlendFactor::SrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        case BlendFactor::ConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
        case BlendFactor::OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        case BlendFactor::SrcAlphaSaturate: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    	default:
    		panic("Unsupported blend factor");
			return VK_BLEND_FACTOR_MAX_ENUM;
    }
}

static void configurePipeline(VulkanPipelineConfig &cfg, VulkanMaterial *material)
{
	switch (material->polygonMode())
    {
        case PolygonMode::Points:
            cfg.withTopology(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
            cfg.withPolygonMode(VK_POLYGON_MODE_POINT);
            break;
        case PolygonMode::Fill:
            cfg.withTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
            cfg.withPolygonMode(VK_POLYGON_MODE_FILL);
            break;
        case PolygonMode::Wireframe:
            cfg.withTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
            cfg.withPolygonMode(VK_POLYGON_MODE_LINE);
            break;
        default:
            panic("Unsupported polygon mode");
    }

    switch (material->faceCull())
    {
        case FaceCull::None:
            cfg.withCullMode(VK_CULL_MODE_NONE);
            break;
        case FaceCull::Front:
            cfg.withCullMode(VK_CULL_MODE_FRONT_BIT);
            break;
        case FaceCull::Back:
            cfg.withCullMode(VK_CULL_MODE_BACK_BIT);
            break;
        default:
            panic("Unsupported face cull mode");
    }

    cfg.withDepthTest(material->hasDepthWrite(), material->hasDepthTest());

    cfg.withBlend(
		material->hasBlend(),
        toBlendFactor(material->srcBlendFactor()),
        toBlendFactor(material->dstBlendFactor()),
        toBlendFactor(material->srcBlendFactor()),
        toBlendFactor(material->dstBlendFactor()));
}

static void configurePipeline(VulkanPipelineConfig &cfg, VulkanMesh *mesh, VulkanEffect *effect)
{
    const auto &effectVertexAttrs = effect->vertexAttributes();

    for (u32 binding = 0; binding < mesh->vertexBufferCount(); binding++)
    {
        const auto &layout = mesh->vertexBufferLayout(binding);
        
        cfg.withVertexBinding(binding, layout.size(), VK_VERTEX_INPUT_RATE_VERTEX);

        u32 offset = 0;
        for (u32 attrIndex = 0; attrIndex < layout.attributeCount(); attrIndex++)
        {
            const auto attr = layout.attribute(attrIndex);
            const auto format = toVertexFormat(attr);
            
            u32 location = 0;
            auto found = true;
            if (!attr.name.empty())
            {
                if (effectVertexAttrs.count(attr.name))
                    location = effectVertexAttrs.at(attr.name).location;
                else
                    found = false;
            }
            if (found)
                cfg.withVertexAttribute(location, binding, format, offset);

            offset += attr.size;
        }
    }
}

VulkanPipelineContext::VulkanPipelineContext(VulkanDriverDevice *device, size_t key):
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
		VulkanDescriptorSetConfig cfg;

		for (const auto &pair: effect->uniformBuffers())
		{
			const auto bufferName = pair.first;
			uniformBuffers_[bufferName] = VulkanBuffer::uniformHostVisible(*device_, pair.second.size);
			cfg.addUniformBuffer(pair.second.binding);
		}

		for (const auto &pair: effect->samplers())
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

		configurePipeline(pipelineConfig, material);
		configurePipeline(pipelineConfig, mesh, effect);

		pipeline_ = VulkanPipeline(*device_, *renderPass, pipelineConfig);
		lastMaterialStateHash_ = materialStateHash;
		lastMeshLayoutHash_ = meshLayoutHash;
	}

	// TODO Run set updater not so often - only when something really changes

	// TODO Not necessary (?), buffers don't change anyway, only their content
	for (auto &pair: uniformBuffers_)
	{
		const auto &info = effect->uniformBuffers().at(pair.first);
		descSet_.updateUniformBuffer(info.binding, pair.second, 0, info.size); // TODO use single large buffer?
	}

	for (const auto &pair: material->samplers())
	{
		const auto &info = pair.second;
		descSet_.updateSampler(
			info.binding,
			info.texture->image().view(),
			info.texture->sampler(),
			info.texture->image().layout());
	}

	for (const auto &p: material->bufferItems())
	{
		auto &buffer = uniformBuffers_[p.first];
		for (const auto &pp: p.second)
			pp.second.write(buffer, camera, transform);
	}
}

#endif
