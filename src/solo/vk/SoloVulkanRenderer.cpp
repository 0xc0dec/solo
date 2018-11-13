/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanRenderer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloHash.h"
#include "SoloVulkanFrameBuffer.h"
#include "SoloVulkanSDLDevice.h"
#include "SoloVulkanMaterial.h"
#include "SoloVulkanMesh.h"
#include "SoloVulkanEffect.h"
#include "SoloVulkanTexture.h"
#include "SoloCamera.h"

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
            SL_DEBUG_PANIC(true, "Unsupported vertex attribute element count");
            return VK_FORMAT_UNDEFINED;
    }
}

static auto genPipelineContextKey(Transform *transform, Camera *camera, VulkanMaterial *material, VkRenderPass renderPass)
{
    size_t seed = 0;
    const std::hash<void*> hasher;
    combineHash(seed, hasher(transform));
    combineHash(seed, hasher(material));
    combineHash(seed, hasher(camera));
    combineHash(seed, hasher(renderPass));
    return seed;
}

VulkanRenderer::VulkanRenderer(Device *engineDevice):
    engineDevice_(engineDevice)
{
    const auto vulkanDevice = dynamic_cast<VulkanSDLDevice*>(engineDevice);
    const auto instance = vulkanDevice->instance();
    const auto surface = vulkanDevice->surface();
    const auto canvasSize = engineDevice->canvasSize();

    device_ = VulkanDevice(instance, surface);
    swapchain_ = VulkanSwapchain(device_, static_cast<u32>(
        canvasSize.x()), static_cast<u32>(canvasSize.y()), engineDevice->isVsync());
}

void VulkanRenderer::beginCamera(Camera *camera, FrameBuffer *renderTarget)
{
    currentCamera_ = camera;
    currentRenderPass_ = &swapchain_.renderPass();
    
    auto currentFrameBuffer = swapchain_.currentFrameBuffer();
    auto dimensions = engineDevice_->canvasSize();

    if (renderTarget)
    {
        const auto targetFrameBuffer = static_cast<VulkanFrameBuffer*>(renderTarget);
        currentRenderPass_ = &targetFrameBuffer->renderPass();
        currentFrameBuffer = targetFrameBuffer->handle();
        dimensions = targetFrameBuffer->dimensions();
    }

    if (!renderPassContexts_.count(currentRenderPass_))
    {
        renderPassContexts_[currentRenderPass_].cmdBuf = VulkanCmdBuffer(device_);
        renderPassContexts_[currentRenderPass_].completeSemaphore = vk::createSemaphore(device_);
    }

    auto &passContext = renderPassContexts_.at(currentRenderPass_);
    passContext.frameOfLastUse = frame_;
    
    currentCmdBuffer_ = &passContext.cmdBuf;
    currentCmdBuffer_->begin(false);

    currentCmdBuffer_->beginRenderPass(*currentRenderPass_, currentFrameBuffer,
        static_cast<u32>(dimensions.x()), static_cast<u32>(dimensions.y()));

    if (currentCamera_->hasColorClearing())
    {
        const VkClearRect clearRect{{{0, 0}, {static_cast<u32>(dimensions.x()), static_cast<u32>(dimensions.y())}}, 0, 1};
        auto clearColor = currentCamera_->clearColor();
        const VkClearValue clearValue{{clearColor.x(), clearColor.y(), clearColor.z(), clearColor.w()}};
        const auto clearCount = renderTarget
            ? static_cast<VulkanFrameBuffer*>(renderTarget)->colorAttachmentCount()
            : 1;
        for (u32 i = 0; i < clearCount; i++)
            currentCmdBuffer_->clearColorAttachment(i, clearValue, clearRect);
    }

    const auto viewport = currentCamera_->viewport();
    currentCmdBuffer_->setViewport(viewport, 0, 1);
    currentCmdBuffer_->setScissor(viewport);
}

void VulkanRenderer::endCamera(Camera *camera, FrameBuffer *renderTarget)
{
    auto &ctx = renderPassContexts_.at(currentRenderPass_);
    ctx.cmdBuf.endRenderPass();
    ctx.cmdBuf.end();

    vk::queueSubmit(device_.queue(), 1, &prevSemaphore_, 1, &ctx.completeSemaphore, 1, ctx.cmdBuf);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(device_.queue()));

    prevSemaphore_ = ctx.completeSemaphore;

    currentCamera_ = nullptr;
}

void VulkanRenderer::drawMesh(Mesh *mesh, Transform *transform, Material *material)
{
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);
    prepareAndBindMesh(material, transform, mesh);

    if (vkMesh->partCount())
    {
        for (u32 part = 0; part < vkMesh->partCount(); part++)
        {
            const auto indexBuffer = vkMesh->partBuffer(part);
            currentCmdBuffer_->bindIndexBuffer(indexBuffer, 0, VK_INDEX_TYPE_UINT16);
            currentCmdBuffer_->drawIndexed(vkMesh->partIndexElementCount(part), 1, 0, 0, 0);
        }
    }
    else
        currentCmdBuffer_->draw(vkMesh->minVertexCount(), 1, 0, 0);
}

void VulkanRenderer::drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material)
{
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);
    prepareAndBindMesh(material, transform, mesh);
    const auto indexBuffer = vkMesh->partBuffer(part);
    currentCmdBuffer_->bindIndexBuffer(indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    currentCmdBuffer_->drawIndexed(vkMesh->partIndexElementCount(part), 1, 0, 0, 0);
}

auto VulkanRenderer::ensurePipelineContext(Transform *transform, VulkanMaterial *material, VulkanMesh *mesh) -> PipelineContext&
{
    const auto vkMaterial = static_cast<VulkanMaterial*>(material);
    const auto vkEffect = static_cast<VulkanEffect*>(vkMaterial->effect().get());
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);

    const auto key = genPipelineContextKey(transform, currentCamera_, material, *currentRenderPass_);
    auto &context = pipelineContexts_[key];

    if (!context.descSet)
    {
        const auto &uniformBufs = vkEffect->uniformBuffers();
        const auto &effectSamplers = vkEffect->samplers();

        VulkanDescriptorSetConfig cfg;

        for (const auto &pair : uniformBufs)
        {
            const auto bufferName = pair.first;
            context.uniformBuffers[bufferName] = VulkanBuffer::uniformHostVisible(device_, pair.second.size);
            cfg.addUniformBuffer(pair.second.binding);
        }

        for (auto &pair : effectSamplers)
            cfg.addSampler(pair.second.binding);

        context.descSet = VulkanDescriptorSet(device_, cfg);
    }

    const auto materialFlagsHash = vkMaterial->stateHash();
    const auto meshLayoutHash = vkMesh->layoutHash();
    const auto materialFlagsChanged = materialFlagsHash != context.lastMaterialFlagsHash;
    const auto meshLayoutChanged = meshLayoutHash != context.lastMeshLayoutHash;

    if (!context.pipeline || materialFlagsChanged || meshLayoutChanged)
    {
        const auto vs = vkEffect->vsModule();
        const auto fs = vkEffect->fsModule();
        auto pipelineConfig = VulkanPipelineConfig(vs, fs)
            .withDescriptorSetLayout(context.descSet.layout())
            .withFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
            .withColorBlendAttachmentCount(currentRenderPass_->colorAttachmentCount());
        vkMaterial->configurePipeline(pipelineConfig);

        const auto &effectVertexAttrs = vkEffect->vertexAttributes();
        for (u32 binding = 0; binding < vkMesh->vertexBufferCount(); binding++)
        {
            const auto &layout = vkMesh->vertexBufferLayout(binding);
            
            pipelineConfig.withVertexBinding(binding, layout.size(), VK_VERTEX_INPUT_RATE_VERTEX);

            u32 offset = 0;
            for (u32 attrIndex = 0; attrIndex < layout.attributeCount(); attrIndex++)
            {
                const auto attr = layout.attribute(attrIndex);
                const auto format = toVertexFormat(attr);
                
                auto location = attr.location;
                auto found = true;
                if (!attr.name.empty())
                {
                    if (effectVertexAttrs.count(attr.name))
                        location = effectVertexAttrs.at(attr.name).location;
                    else
                        found = false;
                }
                if (found)
                    pipelineConfig.withVertexAttribute(location, binding, format, offset);

                offset += attr.size;
            }
        }

        context.pipeline = VulkanPipeline{device_, *currentRenderPass_, pipelineConfig};
        context.lastMaterialFlagsHash = materialFlagsHash;
        context.lastMeshLayoutHash = meshLayoutHash;
    }

    return context;
}

void VulkanRenderer::prepareAndBindMesh(Material *material, Transform *transform, Mesh *mesh)
{
    const auto vkMaterial = static_cast<VulkanMaterial*>(material);
    const auto vkEffect = static_cast<VulkanEffect*>(vkMaterial->effect().get());
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);
    const auto &uniformBufs = vkEffect->uniformBuffers();
    const auto &materialSamplers = vkMaterial->samplers();
    
    auto &context = ensurePipelineContext(transform, vkMaterial, vkMesh);
    context.frameOfLastUse = frame_;

    // Run the desc set updater
    // TODO Not so often - only when something really changes
    
    // TODO Not necessary (?), buffers don't change anyway, only their content
    for (auto &pair : context.uniformBuffers)
    {
        const auto& info = uniformBufs.at(pair.first);
        auto &buffer = pair.second;
        context.descSet.updateUniformBuffer(info.binding, buffer, 0, info.size); // TODO use single large buffer?
    }

    for (auto &pair : materialSamplers)
    {
        auto &info = pair.second;
        context.descSet.updateSampler(
            info.binding,
            info.texture->image().view(),
            info.texture->sampler(),
            info.texture->image().layout());
    }

    // Update buffers content

    auto &bufferItems = vkMaterial->bufferItems();
    for (auto &p: bufferItems)
    {
        auto &buffer = context.uniformBuffers[p.first];
        for (auto &pp: p.second)
            pp.second.write(buffer, currentCamera_, transform);
    }

    // Issue commands

    currentCmdBuffer_->bindPipeline(context.pipeline);
    currentCmdBuffer_->bindDescriptorSet(context.pipeline.layout(), context.descSet);

    for (u32 i = 0; i < vkMesh->vertexBufferCount(); i++)
        currentCmdBuffer_->bindVertexBuffer(i, vkMesh->vertexBuffer(i));
}

void VulkanRenderer::beginFrame()
{
    frame_++;
    currentCamera_ = nullptr;
    currentRenderPass_ = nullptr;
    currentCmdBuffer_ = nullptr;
    prevSemaphore_ = swapchain_.moveNext();
}

void VulkanRenderer::endFrame()
{
    swapchain_.present(device_.queue(), 1, &prevSemaphore_);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(device_.queue()));

    // Naive cleanup TODO better
    if (frame_ % 100 == 0)
    {
        cleanupUnusedPipelineContexts();
        cleanupUnusedRenderPassContexts();
    }
}

void VulkanRenderer::cleanupUnusedRenderPassContexts()
{
    bool removed;
    do
    {
        removed = false;
        VulkanRenderPass* key = nullptr;
        for (const auto &p: renderPassContexts_)
        {
            const auto frameOfLastUse = p.second.frameOfLastUse;
            if ((frame_ > frameOfLastUse && frame_ - frameOfLastUse >= 100) ||
                (frame_ < frameOfLastUse && frameOfLastUse - frame_ >= 100))
            {
                key = p.first;
                removed = true;
            }
        }
        if (removed)
            renderPassContexts_.erase(key);
    }
    while (removed);
}

void VulkanRenderer::cleanupUnusedPipelineContexts()
{
    bool removed;
    do
    {
        removed = false;
        size_t key = 0;
        for (const auto &p: pipelineContexts_)
        {
            const auto frameOfLastUse = p.second.frameOfLastUse;
            if ((frame_ > frameOfLastUse && frame_ - frameOfLastUse >= 100) ||
                (frame_ < frameOfLastUse && frameOfLastUse - frame_ >= 100))
            {
                key = p.first;
                removed = true;
            }
        }
        if (removed)
            pipelineContexts_.erase(key);
    }
    while (removed);
}

#endif
