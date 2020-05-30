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
#include "SoloCamera.h"
#include "SoloVulkanPipelineContext.h"

using namespace solo;

static auto contextKey(Transform *transform, Camera *camera, VulkanMaterial *material, VkRenderPass renderPass)
{
    size_t seed = 0;
    const std::hash<void*> hasher;
    combineHash(seed, hasher(transform));
    combineHash(seed, hasher(material));
    combineHash(seed, hasher(camera));
    combineHash(seed, hasher(renderPass));
    return seed;
}

static auto toIndexType(IndexElementSize elementSize) -> VkIndexType
{
	switch (elementSize)
	{
		case IndexElementSize::Bits16: return VK_INDEX_TYPE_UINT16;
		case IndexElementSize::Bits32: return VK_INDEX_TYPE_UINT32;
	}

	debugPanicIf(true, "Unknown index element size");
	return VK_INDEX_TYPE_MAX_ENUM;
}

VulkanRenderer::VulkanRenderer(Device *device):
    engineDevice_(device)
{
    const auto vkDevice = dynamic_cast<VulkanSDLDevice*>(device);
    const auto instance = vkDevice->instance();
    const auto surface = vkDevice->surface();
    const auto canvasSize = device->canvasSize();

    device_ = VulkanDevice(instance, surface);
    swapchain_ = VulkanSwapchain(device_, static_cast<u32>(canvasSize.x()), static_cast<u32>(canvasSize.y()),
		device->isVsync());
}

void VulkanRenderer::beginCamera(Camera *camera)
{
	const auto renderTarget = camera->renderTarget().get();
	const auto targetFrameBuffer = dynamic_cast<VulkanFrameBuffer*>(renderTarget);
    const auto currentFrameBuffer = targetFrameBuffer ? targetFrameBuffer->handle() : swapchain_.currentFrameBuffer();
    const auto dimensions = targetFrameBuffer ? targetFrameBuffer->dimensions() : engineDevice_->canvasSize();

	currentCamera_ = camera;
	currentRenderPass_ = targetFrameBuffer ? &targetFrameBuffer->renderPass() : &swapchain_.renderPass();
	
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
            ? dynamic_cast<VulkanFrameBuffer*>(renderTarget)->colorAttachmentCount()
            : 1;
        for (u32 i = 0; i < clearCount; i++)
            currentCmdBuffer_->clearColorAttachment(i, clearValue, clearRect);
    }

    const auto viewport = currentCamera_->viewport();
    currentCmdBuffer_->setViewport(viewport, 0, 1);
    currentCmdBuffer_->setScissor(viewport);
}

void VulkanRenderer::endCamera(Camera *camera)
{
    auto &ctx = renderPassContexts_.at(currentRenderPass_);
    ctx.cmdBuf.endRenderPass();
    ctx.cmdBuf.end();

    vk::queueSubmit(device_.queue(), 1, &prevSemaphore_, 1, &ctx.completeSemaphore, 1, ctx.cmdBuf);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(device_.queue()));

    prevSemaphore_ = ctx.completeSemaphore;

    currentCamera_ = nullptr;
}

void VulkanRenderer::renderMesh(Mesh *mesh, Transform *transform, Material *material)
{
    const auto vkMesh = dynamic_cast<VulkanMesh*>(mesh);
    bindPipelineAndMesh(material, transform, mesh);
    currentCmdBuffer_->draw(vkMesh->minVertexCount(), 1, 0, 0);
}

void VulkanRenderer::renderMeshIndex(Mesh *mesh, u32 index, Transform *transform, Material *material)
{
    const auto vkMesh = dynamic_cast<VulkanMesh*>(mesh);
    bindPipelineAndMesh(material, transform, mesh);
    const auto indexBuffer = vkMesh->indexBuffer(index);
	const auto indexType = toIndexType(mesh->indexBufferElementSize(index));
    currentCmdBuffer_->bindIndexBuffer(indexBuffer, 0, indexType);
    currentCmdBuffer_->drawIndexed(vkMesh->indexBufferElementCount(index), 1, 0, 0, 0);
}

void VulkanRenderer::bindPipelineAndMesh(Material *material, Transform *transform, Mesh *mesh)
{
    const auto vkMaterial = dynamic_cast<VulkanMaterial*>(material);
    const auto vkMesh = dynamic_cast<VulkanMesh*>(mesh);

	const auto key = contextKey(transform, currentCamera_, vkMaterial, *currentRenderPass_);
	if (!pipelineContexts_.count(key))
		pipelineContexts_.emplace(std::make_pair(key, VulkanPipelineContext(&device_, key)));

	auto &context = pipelineContexts_.at(key);
	context.setFrameOfLastUse(frame_);
    
    if (currentPipelineContextKey_ != context.key())
    {
    	context.update(vkMaterial, vkMesh, currentRenderPass_, currentCamera_, transform);
        currentCmdBuffer_->bindPipeline(context.pipeline());
        currentCmdBuffer_->bindDescriptorSet(context.pipeline().layout(), context.descriptorSet());
        currentPipelineContextKey_ = context.key();
    }

    // TODO don't rebind an already bound mesh (for instance when we render mesh indexes)
    for (u32 i = 0; i < vkMesh->vertexBufferCount(); i++)
        currentCmdBuffer_->bindVertexBuffer(i, vkMesh->vertexBuffer(i));
}

void VulkanRenderer::beginFrame()
{
    frame_++;
    currentCamera_ = nullptr;
    currentRenderPass_ = nullptr;
    currentCmdBuffer_ = nullptr;
    currentPipelineContextKey_ = 0;
    prevSemaphore_ = swapchain_.moveNext();
}

void VulkanRenderer::endFrame()
{
    swapchain_.present(device_.queue(), 1, &prevSemaphore_);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(device_.queue()));

    // TODO Naive cleanup, need better
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
	        if (frame_ - p.second.frameOfLastUse >= 100)
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
	        if (frame_ - p.second.frameOfLastUse() >= 100)
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
