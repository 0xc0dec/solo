/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
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

using namespace solo;

static auto parseName(const str &name) -> std::tuple<str, str> {
    const auto idx = name.find(':');
    const auto first = (idx != str::npos) ? name.substr(0, idx) : name;
    const auto second = (idx != str::npos) ? name.substr(idx + 1) : "";
    return make_tuple(first, second);
}

VulkanMaterial::VulkanMaterial(const sptr<Effect> &effect):
    effect_(std::static_pointer_cast<VulkanEffect>(effect)) {
}

auto VulkanMaterial::stateHash() const -> size_t {
    size_t seed = 0;
    const std::hash<u32> unsignedHasher;
    const std::hash<bool> boolHash;
    combineHash(seed, unsignedHasher(static_cast<u32>(faceCull_)));
    combineHash(seed, unsignedHasher(static_cast<u32>(polygonMode_)));
    combineHash(seed, unsignedHasher(static_cast<u32>(srcBlendFactor_)));
    combineHash(seed, unsignedHasher(static_cast<u32>(dstBlendFactor_)));
    combineHash(seed, boolHash(depthTest_));
    combineHash(seed, boolHash(depthWrite_));
    return seed;
}

void VulkanMaterial::setFloatParameter(const str &name, float value) {
    setParameter(name, [value](auto & buffer, auto offset, auto size, auto, auto) {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setVector2Parameter(const str &name, const Vector2 &value) {
    setParameter(name, [value](auto & buffer, auto offset, auto size, auto, auto) {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setVector3Parameter(const str &name, const Vector3 &value) {
    setParameter(name, [value](auto & buffer, auto offset, auto size, auto, auto) {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setVector4Parameter(const str &name, const Vector4 &value) {
    setParameter(name, [value](auto & buffer, auto offset, auto size, auto, auto) {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setMatrixParameter(const str &name, const Matrix &value) {
    // TODO avoid copy-paste
    setParameter(name, [value](auto & buffer, auto offset, auto size, auto, auto) {
        buffer.updatePart(&value, offset, size);
    });
}

void VulkanMaterial::setParameter(const str &name, const ParameterWriteFunc &write) {
    const auto parsedName = parseName(name);
    const auto &bufferName = std::get<0>(parsedName);
    const auto &fieldName = std::get<1>(parsedName);

    panicIf(bufferName.empty() || fieldName.empty(), "Invalid material parameter name ", name);

    if (effect_->hasUniformBuffer(bufferName)) {
        auto bufferInfo = effect_->uniformBuffer(bufferName);
        if (bufferInfo.members.count(fieldName)) {
            const auto itemInfo = bufferInfo.members.at(fieldName);
            auto &item = bufferItems_[bufferName][fieldName];

            item.write = [itemInfo, write](VulkanBuffer & buffer, const Camera * camera, const Transform * transform) {
                write(buffer, itemInfo.offset, itemInfo.size, camera, transform);
            };

            return;
        }
    }

    panic("Unknown material parameter ", name);
}

void VulkanMaterial::setTextureParameter(const str &name, sptr<Texture> value) {
    panicIf(!effect_->hasSampler(name), "Unknown material parameter ", name);
    const auto samplerInfo = effect_->sampler(name);
    auto &sampler = samplers_[name];
    sampler.binding = samplerInfo.binding;
    sampler.texture = std::dynamic_pointer_cast<VulkanTexture>(value);
    // TODO Optimize and mark only this sampler as dirty
}

void VulkanMaterial::bindFloatParameter(const str &name, const std::function<float()> &valueGetter) {
    setParameter(name, [valueGetter](auto & buffer, auto offset, auto size, auto, auto) {
        const auto val = valueGetter();
        buffer.updatePart(&val, offset, size);
    });
}

void VulkanMaterial::bindVector2Parameter(const str &name, const std::function<Vector2()> &valueGetter) {
    setParameter(name, [valueGetter](auto & buffer, auto offset, auto size, auto, auto) {
        const auto val = valueGetter();
        buffer.updatePart(&val, offset, size);
    });
}

void VulkanMaterial::bindVector3Parameter(const str &name, const std::function<Vector3()> &valueGetter) {
    setParameter(name, [valueGetter](auto & buffer, auto offset, auto size, auto, auto) {
        const auto val = valueGetter();
        buffer.updatePart(&val, offset, size);
    });
}

void VulkanMaterial::bindVector4Parameter(const str &name, const std::function<Vector4()> &valueGetter) {
    setParameter(name, [valueGetter](auto & buffer, auto offset, auto size, auto, auto) {
        const auto val = valueGetter();
        buffer.updatePart(&val, offset, size);
    });
}

void VulkanMaterial::bindMatrixParameter(const str &name, const std::function<Matrix()> &valueGetter) {
    setParameter(name, [valueGetter](auto & buffer, auto offset, auto size, auto, auto) {
        const auto val = valueGetter();
        buffer.updatePart(&val, offset, size);
    });
}

void VulkanMaterial::bindParameter(const str &name, ParameterBinding binding) {
    switch (binding) {
    case ParameterBinding::WorldMatrix: {
            setParameter(name, [](VulkanBuffer & buffer, u32 offset, u32 size, const Camera *, const Transform * nodeTransform) {
                if (nodeTransform) {
                    auto value = nodeTransform->worldMatrix();
                    buffer.updatePart(&value, offset, size);
                }
            });
            break;
        }

    case ParameterBinding::ViewMatrix: {
            setParameter(name, [](VulkanBuffer & buffer, u32 offset, u32 size, const Camera * camera, const Transform *) {
                if (camera) {
                    auto value = camera->viewMatrix();
                    buffer.updatePart(&value, offset, size);
                }
            });
            break;
        }

    case ParameterBinding::ProjectionMatrix: {
            setParameter(name, [](VulkanBuffer & buffer, u32 offset, u32 size, const Camera * camera, const Transform *) {
                if (camera) {
                    auto value = camera->projectionMatrix();
                    buffer.updatePart(&value, offset, size);
                }
            });
            break;
        }

    case ParameterBinding::WorldViewMatrix: {
            setParameter(name, [](VulkanBuffer & buffer, u32 offset, u32 size, const Camera * camera, const Transform * nodeTransform) {
                if (camera && nodeTransform) {
                    auto value = nodeTransform->worldViewMatrix(camera);
                    buffer.updatePart(&value, offset, size);
                }
            });
            break;
        }

    case ParameterBinding::ViewProjectionMatrix: {
            setParameter(name, [](VulkanBuffer & buffer, u32 offset, u32 size, const Camera * camera, const Transform * nodeTransform) {
                if (camera) {
                    auto value = camera->viewProjectionMatrix();
                    buffer.updatePart(&value, offset, size);
                }
            });
            break;
        }

    case ParameterBinding::WorldViewProjectionMatrix: {
            setParameter(name, [](VulkanBuffer & buffer, u32 offset, u32 size, const Camera * camera, const Transform * nodeTransform) {
                if (nodeTransform && camera) {
                    auto value = nodeTransform->worldViewProjMatrix(camera);
                    buffer.updatePart(&value, offset, size);
                }
            });
            break;
        }

    case ParameterBinding::InverseTransposedWorldMatrix: {
            setParameter(name, [](VulkanBuffer & buffer, u32 offset, u32 size, const Camera *, const Transform * nodeTransform) {
                if (nodeTransform) {
                    auto value = nodeTransform->invTransposedWorldMatrix();
                    buffer.updatePart(&value, offset, size);
                }
            });
            break;
        }

    case ParameterBinding::InverseTransposedWorldViewMatrix: {
            setParameter(name, [](VulkanBuffer & buffer, u32 offset, u32 size, const Camera * camera, const Transform * nodeTransform) {
                if (nodeTransform && camera) {
                    auto value = nodeTransform->invTransposedWorldViewMatrix(camera);
                    buffer.updatePart(&value, offset, size);
                }
            });
            break;
        }

    case ParameterBinding::CameraWorldPosition: {
            setParameter(name, [](VulkanBuffer & buffer, u32 offset, u32 size, const Camera * camera, const Transform *) {
                if (camera) {
                    auto value = camera->transform()->worldPosition();
                    buffer.updatePart(&value, offset, size);
                }
            });
            break;
        }

    default:
        panic("Unsupported parameter binding");
    }
}

#endif
