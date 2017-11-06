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
#include "SoloVulkanTexture.h"

using namespace solo;

static auto parseName(const str &name) -> std::tuple<str, str>
{
    const auto idx = name.find(".");
    const auto first = (idx != str::npos) ? name.substr(0, idx) : name;
    const auto second = (idx != str::npos) ? name.substr(idx + 1) : "";
    return make_tuple(first, second);
}

vk::Material::Material(sptr<solo::Effect> effect):
    effect(std::dynamic_pointer_cast<Effect>(effect))
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

void vk::Material::setFloatParameter(const str &name, float value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setVector2Parameter(const str &name, const Vector2 &value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setVector3Parameter(const str &name, const Vector3 &value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setVector4Parameter(const str &name, const Vector4 &value)
{
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setMatrixParameter(const str &name, const Matrix &value)
{
    // TODO avoid copy-paste
    setUniformParameter(name, [value](auto &buffer, auto offset, auto size, auto, auto)
    {
        buffer.updatePart(&value, offset, size);
    });
}

void vk::Material::setUniformParameter(const str &name, ParameterWriteFunc write)
{
    auto parsedName = parseName(name);
    auto bufferName = std::get<0>(parsedName);
    auto fieldName = std::get<1>(parsedName);
    SL_PANIC_IF(bufferName.empty() || fieldName.empty(), SL_FMT("Invalid parameter name ", name));

    auto bufferInfo = effect->getUniformBuffer(bufferName);
    const auto itemInfo = bufferInfo.members.at(fieldName);

    auto &item = bufferItems[bufferName][fieldName];
    item.write = [itemInfo, write](Buffer &buffer, const Camera *camera, const Transform *transform)
    {
        write(buffer, itemInfo.offset, itemInfo.size, camera, transform);
    };
}

void vk::Material::setTextureParameter(const str &name, sptr<solo::Texture> value)
{
    const auto samplerInfo = effect->getSampler(name);
    auto &sampler = samplers[name];
    sampler.binding = samplerInfo.binding;
    sampler.texture = std::dynamic_pointer_cast<Texture>(value);
    // TODO Optimize and mark only this sampler as dirty
}

void vk::Material::bindParameter(const str &name, BindParameterSemantics semantics)
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

#endif
