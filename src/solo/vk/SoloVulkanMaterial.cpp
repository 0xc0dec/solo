/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanMaterial.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanDescriptorSetLayoutBuilder.h"
#include "SoloVulkanDescriptorSetUpdater.h"

using namespace solo;

vk::Material::Material(sptr<Effect> effect):
    solo::Material(effect)
{
}

vk::Material::~Material()
{
}

void vk::Material::setFloatParameter(const std::string &name, float value)
{
}

void vk::Material::setFloatArrayParameter(const std::string &name, const std::vector<float> &value)
{
}

void vk::Material::setVector2Parameter(const std::string &name, const Vector2 &value)
{
}

void vk::Material::setVector2ArrayParameter(const std::string &name, const std::vector<Vector2> &value)
{
}

void vk::Material::setVector3Parameter(const std::string &name, const Vector3 &value)
{
}

void vk::Material::setVector3ArrayParameter(const std::string &name, const std::vector<Vector3> &value)
{
}

void vk::Material::setVector4Parameter(const std::string &name, const Vector4 &value)
{
}

void vk::Material::setVector4ArrayParameter(const std::string &name, const std::vector<Vector4> &value)
{
}

void vk::Material::setMatrixParameter(const std::string &name, const Matrix &value)
{
}

void vk::Material::setMatrixArrayParameter(const std::string &name, const std::vector<Matrix> &value)
{
}

void vk::Material::setTextureParameter(const std::string &name, sptr<Texture> value)
{
}

void vk::Material::bindWorldMatrixParameter(const std::string &name)
{
}

void vk::Material::bindViewMatrixParameter(const std::string &name)
{
}

void vk::Material::bindProjectionMatrixParameter(const std::string &name)
{
}

void vk::Material::bindWorldViewMatrixParameter(const std::string &name)
{
}

void vk::Material::bindViewProjectionMatrixParameter(const std::string &name)
{
}

void vk::Material::bindWorldViewProjectionMatrixParameter(const std::string &name)
{
}

void vk::Material::bindInvTransposedWorldMatrixParameter(const std::string &name)
{
}

void vk::Material::bindInvTransposedWorldViewMatrixParameter(const std::string &name)
{
}

void vk::Material::bindCameraWorldPositionParameter(const std::string &name)
{
}

void vk::Material::applyParameters(Renderer *renderer)
{
    if (dirtyLayout)
    {
        // Store everything in one set for simplicity
        // "0:2" means "binding 0, index 2 in the uniform buffer" (for uniforms)
        // "1" means "set 0, sampler binding 1"

        struct UniformBufferItem
        {
            
        };

        struct Binding
        {
            std::unordered_map<uint32_t, UniformBufferItem> bufferItems;
            // TODO texture pointer here
        };

        std::unordered_map<uint32_t, Binding> bindings;

        std::unordered_map<uint32_t, float> floatParams;
        std::unordered_map<uint32_t, sptr<Texture>> textureParams;
        std::unordered_map<uint32_t, std::function<void*()>> getters;
        std::vector<uint32_t> uniformSizes;

        auto builder = vk::DescriptorSetLayoutBuilder(renderer->getDevice());

        for (const auto &b: bindings)
        {
            if (!b.second.bufferItems.empty())
                builder.withBinding(b.first, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL_GRAPHICS);
        }

        for (auto &p: textureParams)
            builder.withBinding(p.first, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);

        // TODO continue below

        const auto samplerCount = textureParams.size();

        descSetLayout = builder.build();

        auto poolConfig = vk::DescriptorPoolConfig();
        if (anyUniforms)
            // Convension: store all uniforms in one buffer for simplicity
            poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1);
        if (samplerCount > 0)
            poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, samplerCount);

        // TODO Make sure this destroys the old pool
        descPool = vk::DescriptorPool(renderer->getDevice(), 1, poolConfig);

        descSet = descPool.allocateSet(descSetLayout);

        if (anyUniforms)
        {
            auto newBufferSize = 0;
            for (auto size: uniformSizes)
            {
                newBufferSize += size;
                if (!size)
                    break;
            }

            if (newBufferSize != uniformBuffer.getSize())
                // TODO Make sure the old buffer is destroyed
                uniformBuffer = Buffer::createUniformHostVisible(renderer, newBufferSize);
        }

        vk::DescriptorSetUpdater(renderer->getDevice())
            .forUniformBuffer()
            .forTexture(0, descSet, colorAttachment.getView(), colorAttachment.getSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            .updateSets();

        dirtyLayout = false;
    }

    // TODO write parameter values into uniform buffer, then call updateSets()
}

#endif
