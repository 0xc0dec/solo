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

    auto &buffer = uniformBuffers[bufferName];
    buffer.dirty = true;
    buffer.alwaysDirty = false;
    buffer.binding = bufferInfo.binding;
    buffer.size = bufferInfo.size;
    if (!buffer.buffer)
        dirtyLayout = true;

    auto &item = buffer.items[fieldName];
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
    dirtyLayout = true;
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

    auto &buffer = uniformBuffers[bufferName];
    buffer.dirty = true;
    buffer.alwaysDirty = true;
    buffer.binding = bufferInfo.binding;
    buffer.size = bufferInfo.size;
    if (!buffer.buffer)
        dirtyLayout = true;

    auto &item = buffer.items[fieldName];
    item.dirty = true;
    item.alwaysDirty = true;

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
    if (dirtyLayout)
    {
        auto builder = vk::DescriptorSetLayoutBuilder(renderer->getDevice());

        for (auto &pair : uniformBuffers)
        {
            auto &info = pair.second;
            if (!info.buffer)
            {
                builder.withBinding(info.binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL_GRAPHICS);
                // TODO Make sure this destroys the old buffer
                info.buffer = Buffer::createUniformHostVisible(renderer, info.size);
            }
        }

        for (auto &pair : samplers)
        {
            auto &info = pair.second;
            builder.withBinding(info.binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
                VK_SHADER_STAGE_FRAGMENT_BIT);
        }

        descSetLayout = builder.build();

        auto poolConfig = vk::DescriptorPoolConfig();
        if (!uniformBuffers.empty())
            poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniformBuffers.size());
        if (!samplers.empty())
            poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, samplers.size());

        // TODO Make sure this destroys the old pool
        descPool = vk::DescriptorPool(renderer->getDevice(), 1, poolConfig);
        descSet = descPool.allocateSet(descSetLayout);

        vk::DescriptorSetUpdater updater{renderer->getDevice()};

        for (auto &pair : uniformBuffers)
        {
            auto &info = pair.second;
            updater.forUniformBuffer(info.binding, descSet, info.buffer, 0, info.size);
        }

        for (auto &pair : samplers)
        {
            auto &info = pair.second;
            updater.forTexture(info.binding, descSet, info.texture->getView(), info.texture->getSampler(),
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }

        updater.updateSets();

        dirtyLayout = false;
    }

    for (auto &pair : uniformBuffers)
    {
        auto &buffer = pair.second;
        if (buffer.dirty)
        {
            for (auto &p : buffer.items)
            {
                auto &item = p.second;
                if (item.dirty)
                {
                    item.write(buffer.buffer, camera, nodeTransform);
                    item.dirty = item.alwaysDirty;
                }
            }

            buffer.dirty = buffer.alwaysDirty;
        }
    }
}

#endif
