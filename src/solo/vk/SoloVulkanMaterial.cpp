/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanMaterial.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloTransform.h"
#include "SoloCamera.h"
#include "SoloHash.h"
#include "SoloVulkanEffect.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanTexture.h"
#include "SoloVulkanPrefabShaders.h"
#include "SoloVulkanPipeline.h"

using namespace solo;

static auto parseName(const str &name) -> std::tuple<str, str>
{
    const auto idx = name.find(".");
    const auto first = (idx != str::npos) ? name.substr(0, idx) : name;
    const auto second = (idx != str::npos) ? name.substr(idx + 1) : "";
    return make_tuple(first, second);
}

static auto convertBlendFactor(BlendFactor factor) -> VkBlendFactor
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
        // TODO support other vk blend modes?
        default:
            SL_PANIC("Unknown blend factor");
            return VK_BLEND_FACTOR_ONE;
    }
}

auto VulkanMaterial::createFromPrefab(Device *device, MaterialPrefab prefab) -> sptr<VulkanMaterial>
{
    switch (prefab)
    {
        case MaterialPrefab::Font:
        {
            auto effect = Effect::createFromSources(
                device,
                VulkanPrefabShaders::Vertex::font, strlen(VulkanPrefabShaders::Vertex::font),
                VulkanPrefabShaders::Fragment::font, strlen(VulkanPrefabShaders::Fragment::font)
            );
            auto material = std::make_shared<VulkanMaterial>(effect);
            material->bindParameter("matrices.wvp", BindParameterSemantics::WorldViewProjectionMatrix);
            return material;
        }

        case MaterialPrefab::Skybox:
        {
            auto effect = Effect::createFromSources(
                device,
                VulkanPrefabShaders::Vertex::skybox, strlen(VulkanPrefabShaders::Vertex::skybox),
                VulkanPrefabShaders::Fragment::skybox, strlen(VulkanPrefabShaders::Fragment::skybox)
            );
            auto material = std::make_shared<VulkanMaterial>(effect);
            material->bindParameter("matrices.proj", BindParameterSemantics::ProjectionMatrix);
            material->bindParameter("matrices.worldView", BindParameterSemantics::WorldViewMatrix);
            return material;
        }

        default:
            SL_PANIC("Unknown material prefab");
            return nullptr;
    }
}

VulkanMaterial::VulkanMaterial(sptr<Effect> effect):
    effect(std::static_pointer_cast<VulkanEffect>(effect))
{
}

VulkanMaterial::~VulkanMaterial()
{
}

auto VulkanMaterial::getStateHash() const -> size_t
{
    size_t seed = 0;
    const std::hash<u32> unsignedHasher;
    const std::hash<bool> boolHash;
    combineHash(seed, unsignedHasher(static_cast<u32>(faceCull)));
    combineHash(seed, unsignedHasher(static_cast<u32>(polygonMode)));
    combineHash(seed, unsignedHasher(static_cast<u32>(srcBlendFactor)));
    combineHash(seed, unsignedHasher(static_cast<u32>(dstBlendFactor)));
    combineHash(seed, boolHash(depthTest));
    combineHash(seed, boolHash(depthWrite));
    return seed;
}

void VulkanMaterial::configurePipeline(VulkanPipelineConfig &cfg)
{
    switch (polygonMode)
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
            SL_PANIC("Unknown polygon mode");
            break;
    }

    switch (faceCull)
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
            SL_PANIC("Unsupported face cull mode");
            break;
    }

    cfg.withDepthTest(depthWrite, depthTest);

    cfg.withBlend(
        blend,
        convertBlendFactor(srcBlendFactor),
        convertBlendFactor(dstBlendFactor),
        convertBlendFactor(srcBlendFactor),
        convertBlendFactor(dstBlendFactor));
}

void VulkanMaterial::setFloatParameter(const str &name, float value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setVector2Parameter(const str &name, const Vector2 &value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setVector3Parameter(const str &name, const Vector3 &value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setVector4Parameter(const str &name, const Vector4 &value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setMatrixParameter(const str &name, const Matrix &value)
{
    // TODO avoid copy-paste
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setUniformParameter(const str &name, ParameterWriteFunc write)
{
    auto parsedName = parseName(name);
    auto bufferName = std::get<0>(parsedName);
    auto fieldName = std::get<1>(parsedName);
    SL_PANIC_IF(bufferName.empty() || fieldName.empty(), SL_FMT("Invalid parameter name ", name));

    auto bufferInfo = effect->getUniformBuffer(bufferName);
    const auto itemInfo = bufferInfo.members.at(fieldName);

    auto &item = bufferItems[bufferName][fieldName];
    item.write = [itemInfo, write](VulkanBuffer &buffer, const Camera *camera, const Transform *transform)
    {
        write(buffer, itemInfo.offset, itemInfo.size, camera, transform);
    };
}

void VulkanMaterial::setTextureParameter(const str &name, sptr<Texture> value)
{
    const auto samplerInfo = effect->getSampler(name);
    auto &sampler = samplers[name];
    sampler.binding = samplerInfo.binding;
    sampler.texture = std::dynamic_pointer_cast<VulkanTexture>(value);
    // TODO Optimize and mark only this sampler as dirty
}

void VulkanMaterial::bindParameter(const str &name, BindParameterSemantics semantics)
{
    auto parsedName = parseName(name);
    auto bufferName = std::get<0>(parsedName);
    auto fieldName = std::get<1>(parsedName);
    SL_PANIC_IF(bufferName.empty() || fieldName.empty(), SL_FMT("Invalid parameter name ", name));

    auto bufferInfo = effect->getUniformBuffer(bufferName);
    auto itemInfo = bufferInfo.members.at(fieldName);
    auto &item = bufferItems[bufferName][fieldName];

    switch (semantics)
    {
        case BindParameterSemantics::WorldMatrix:
        {
            item.write = [itemInfo](VulkanBuffer &buffer, const Camera *camera, const Transform *nodeTransform)
            {
                if (nodeTransform)
                {
                    auto value = nodeTransform->getWorldMatrix();
                    buffer.updatePart(&value, itemInfo.offset, itemInfo.size);
                }
            };
            break;
        }

        case BindParameterSemantics::ViewMatrix:
        {
            item.write = [itemInfo](VulkanBuffer &buffer, const Camera *camera, const Transform *nodeTransform)
            {
                if (camera)
                {
                    auto value = camera->getViewMatrix();
                    buffer.updatePart(&value, itemInfo.offset, itemInfo.size);
                }
            };
            break;
        }

        case BindParameterSemantics::ProjectionMatrix:
        {
            item.write = [itemInfo](VulkanBuffer &buffer, const Camera *camera, const Transform *nodeTransform)
            {
                if (camera)
                {
                    auto value = camera->getProjectionMatrix();
                    buffer.updatePart(&value, itemInfo.offset, itemInfo.size);
                }
            };
            break;
        }

        case BindParameterSemantics::WorldViewMatrix:
        {
            item.write = [itemInfo](VulkanBuffer &buffer, const Camera *camera, const Transform *nodeTransform)
            {
                if (camera && nodeTransform)
                {
                    auto value = nodeTransform->getWorldViewMatrix(camera);
                    buffer.updatePart(&value, itemInfo.offset, itemInfo.size);
                }
            };
            break;
        }

        case BindParameterSemantics::ViewProjectionMatrix:
        {
            item.write = [itemInfo](VulkanBuffer &buffer, const Camera *camera, const Transform *nodeTransform)
            {
                if (camera)
                {
                    auto value = camera->getViewProjectionMatrix();
                    buffer.updatePart(&value, itemInfo.offset, itemInfo.size);
                }
            };
            break;
        }

        case BindParameterSemantics::WorldViewProjectionMatrix:
        {
            item.write = [itemInfo](VulkanBuffer &buffer, const Camera *camera, const Transform *nodeTransform)
            {
                if (nodeTransform && camera)
                {
                    auto value = nodeTransform->getWorldViewProjMatrix(camera);
                    buffer.updatePart(&value, itemInfo.offset, itemInfo.size);
                }
            };
            break;
        }

        case BindParameterSemantics::InverseTransposedWorldMatrix:
        {
            item.write = [itemInfo](VulkanBuffer &buffer, const Camera *camera, const Transform *nodeTransform)
            {
                if (nodeTransform)
                {
                    auto value = nodeTransform->getInvTransposedWorldMatrix();
                    buffer.updatePart(&value, itemInfo.offset, itemInfo.size);
                }
            };
            break;
        }

        case BindParameterSemantics::InverseTransposedWorldViewMatrix:
        {
            item.write = [itemInfo](VulkanBuffer &buffer, const Camera *camera, const Transform *nodeTransform)
            {
                if (nodeTransform && camera)
                {
                    auto value = nodeTransform->getInvTransposedWorldViewMatrix(camera);
                    buffer.updatePart(&value, itemInfo.offset, itemInfo.size);
                }
            };
            break;
        }

        case BindParameterSemantics::CameraWorldPosition:
        {
            item.write = [itemInfo](VulkanBuffer &buffer, const Camera *camera, const Transform *nodeTransform)
            {
                if (camera)
                {
                    auto value = camera->getTransform()->getWorldPosition();
                    buffer.updatePart(&value, itemInfo.offset, itemInfo.size);
                }
            };
            break;
        }

        default:
            SL_PANIC("Unsupported bind parameter semantics");
    }
}

#endif
