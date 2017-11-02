/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanMaterial.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloTransform.h"
#include "SoloCamera.h"
#include "SoloVulkanEffect.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanDescriptorSetLayoutBuilder.h"
#include "SoloVulkanDescriptorSetUpdater.h"
#include "SoloVulkanTexture.h"

using namespace solo;

static auto parseName(const std::string &name) -> std::tuple<std::string, std::string>
{
    const auto idx = name.find(".");
    const auto first = (idx != std::string::npos) ? name.substr(0, idx) : name;
    const auto second = (idx != std::string::npos) ? name.substr(idx + 1) : "";
    return std::make_tuple(first, second);
}

vk::Material::Material(sptr<solo::Effect> effect):
    effect(std::dynamic_pointer_cast<vk::Effect>(effect))
{
}

vk::Material::~Material()
{
}

auto vk::Material::getCullModeFlags() const -> VkCullModeFlags
{
    switch (faceCull)
    {
        case FaceCull::All: return VK_CULL_MODE_NONE;
        case FaceCull::CCW: return VK_CULL_MODE_FRONT_BIT;
        case FaceCull::CW: return VK_CULL_MODE_BACK_BIT;
        default:
            SL_PANIC("Unsupported face cull mode");
            return VK_FRONT_FACE_CLOCKWISE;
    }
}

auto vk::Material::getVkPolygonMode() const -> VkPolygonMode
{
    switch (polygonMode)
    {
        case PolygonMode::Points: return VK_POLYGON_MODE_POINT;
        case PolygonMode::Triangle: return VK_POLYGON_MODE_FILL;
        case PolygonMode::Wireframe: return VK_POLYGON_MODE_LINE;
        default:
            SL_PANIC("Unsupported polygon mode");
            return VK_POLYGON_MODE_FILL;
    }
}

void vk::Material::setFloatParameter(const std::string &name, float value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setVector2Parameter(const std::string &name, const Vector2 &value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setVector3Parameter(const std::string &name, const Vector3 &value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setVector4Parameter(const std::string &name, const Vector4 &value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setMatrixParameter(const std::string &name, const Matrix &value)
{
    // TODO avoid copy-paste
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setUniformParameter(const std::string &name, ParameterWriteFunc write)
{
    auto parsedName = parseName(name);
    auto bufferName = std::get<0>(parsedName);
    auto fieldName = std::get<1>(parsedName);
    SL_PANIC_IF(bufferName.empty() || fieldName.empty(), SL_FMT("Invalid parameter name ", name));

    auto bufferInfo = effect->getUniformBufferInfo(bufferName);
    const auto itemInfo = bufferInfo.members.at(fieldName);

    auto &item = bufferItems[bufferName][fieldName];
    item.dirty = true;
    item.alwaysDirty = false;
    item.write = [itemInfo, write](Buffer &buffer, const Camera *camera, const Transform *transform)
    {
        write(buffer, itemInfo.offset, itemInfo.size, camera, transform);
    };
}

void vk::Material::setTextureParameter(const std::string &name, sptr<solo::Texture> value)
{
    const auto samplerInfo = effect->getSamplerInfo(name);
    auto &sampler = samplers[name];
    sampler.binding = samplerInfo.binding;
    sampler.texture = std::dynamic_pointer_cast<vk::Texture>(value);
    // TODO Optimize and mark only this sampler as dirty
}

void vk::Material::bindParameter(const std::string &name, BindParameterSemantics semantics)
{
    auto parsedName = parseName(name);
    auto bufferName = std::get<0>(parsedName);
    auto fieldName = std::get<1>(parsedName);
    SL_PANIC_IF(bufferName.empty() || fieldName.empty(), SL_FMT("Invalid parameter name ", name));

    auto bufferInfo = effect->getUniformBufferInfo(bufferName);
    auto itemInfo = bufferInfo.members.at(fieldName);

    auto &item = bufferItems[bufferName][fieldName];
    item.dirty = true;
    item.alwaysDirty = false;

    switch (semantics)
    {
        case BindParameterSemantics::WorldMatrix:
        {
            item.write = [itemInfo](Buffer &buffer, const Camera *camera, const Transform *nodeTransform)
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
            item.write = [itemInfo](Buffer &buffer, const Camera *camera, const Transform *nodeTransform)
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
            item.write = [itemInfo](Buffer &buffer, const Camera *camera, const Transform *nodeTransform)
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
            item.write = [itemInfo](Buffer &buffer, const Camera *camera, const Transform *nodeTransform)
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
            item.write = [itemInfo](Buffer &buffer, const Camera *camera, const Transform *nodeTransform)
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
            item.write = [itemInfo](Buffer &buffer, const Camera *camera, const Transform *nodeTransform)
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
            item.write = [itemInfo](Buffer &buffer, const Camera *camera, const Transform *nodeTransform)
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
            item.write = [itemInfo](Buffer &buffer, const Camera *camera, const Transform *nodeTransform)
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
            item.write = [itemInfo](Buffer &buffer, const Camera *camera, const Transform *nodeTransform)
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

void vk::Material::applyParameters(Renderer *renderer, const Camera *camera, const Transform *nodeTransform)
{
    if (!nodeBindings.count(nodeTransform) || !nodeBindings[nodeTransform].count(camera))
    {
        auto &binding = nodeBindings[nodeTransform][camera];

        auto builder = vk::DescriptorSetLayoutBuilder(renderer->getDevice());

        auto effectBuffers = effect->getUniformBuffers();
        for (const auto &info: effectBuffers)
        {
            const auto bufferName = info.first;
            binding.buffers[bufferName] = Buffer::createUniformHostVisible(renderer, info.second.size);
            builder.withBinding(info.second.binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL_GRAPHICS);
        }

        for (auto &pair : samplers)
            builder.withBinding(pair.second.binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);

        binding.descSetLayout = builder.build();

        auto poolConfig = DescriptorPoolConfig();
        poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, effectBuffers.size());
        poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, samplers.size());

        binding.descPool = vk::DescriptorPool(renderer->getDevice(), 1, poolConfig);
        binding.descSet = binding.descPool.allocateSet(binding.descSetLayout);

        DescriptorSetUpdater updater{renderer->getDevice()};

        for (auto &pair : binding.buffers)
        {
            const auto info = effectBuffers[pair.first];
            auto &buffer = pair.second;
            updater.forUniformBuffer(info.binding, binding.descSet, buffer, 0, info.size); // TODO use single large buffer?
        }

        for (auto &pair : samplers)
        {
            auto &info = pair.second;
            updater.forTexture(info.binding, binding.descSet, info.texture->getView(), info.texture->getSampler(),
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }

        updater.updateSets();
    }

    // TODO Properly mark items as dirty on per-object basis
    auto &binding = nodeBindings[nodeTransform][camera];
    for (auto &p: bufferItems)
    {
        auto &buffer = binding.buffers[p.first];
        for (auto &pp: p.second)
            pp.second.write(buffer, camera, nodeTransform);
    }
}

auto vk::Material::getDescSetLayout(const Camera *camera, const Transform *nodeTransform) const -> VkDescriptorSetLayout
{
    SL_PANIC_IF(!nodeBindings.count(nodeTransform) || !nodeBindings.at(nodeTransform).count(camera), "Node binding not found");
    return nodeBindings.at(nodeTransform).at(camera).descSetLayout;
}

auto vk::Material::getDescSet(const Camera *camera, const Transform *nodeTransform) const -> VkDescriptorSet
{
    SL_PANIC_IF(!nodeBindings.count(nodeTransform) || !nodeBindings.at(nodeTransform).count(camera), "Node binding not found");
    return nodeBindings.at(nodeTransform).at(camera).descSet;
}

#endif
