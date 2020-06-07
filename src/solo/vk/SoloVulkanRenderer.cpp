/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanRenderer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloHash.h"
#include "SoloVulkanFrameBuffer.h"
#include "SoloVulkanDevice.h"
#include "SoloVulkanMaterial.h"
#include "SoloVulkanMesh.h"
#include "SoloCamera.h"
#include "SoloVulkanPipelineContext.h"
#include "SoloVulkanDebugInterface.h"

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
		default:
			panic(false, "Unknown index element size");
			return VK_INDEX_TYPE_MAX_ENUM;
	}
}

VulkanRenderer::VulkanRenderer(Device *device):
	Renderer(device),
    device_(dynamic_cast<VulkanDevice*>(device))
{
	const auto canvasSize = device->canvasSize();

    driverDevice_ = VulkanDriverDevice(device_->instance(), device_->surface());
    swapchain_ = VulkanSwapchain(driverDevice_, static_cast<u32>(canvasSize.x()), static_cast<u32>(canvasSize.y()), device->isVsync());

	context_.debugInterface.completeSemaphore = vk::createSemaphore(driverDevice_);
	context_.debugInterface.renderCmdBuffer = VulkanCmdBuffer(driverDevice_);
}

void VulkanRenderer::beginCamera(Camera *camera)
{
	const auto renderTarget = camera->renderTarget().get();
	const auto targetFrameBuffer = dynamic_cast<VulkanFrameBuffer*>(renderTarget);
    const auto currentFrameBuffer = targetFrameBuffer ? targetFrameBuffer->handle() : swapchain_.currentFrameBuffer();
    const auto dimensions = targetFrameBuffer ? targetFrameBuffer->dimensions() : device_->canvasSize();

	context_.camera = camera;
	context_.renderPass = targetFrameBuffer ? &targetFrameBuffer->renderPass() : &swapchain_.renderPass();
	
    if (!renderPassContexts_.count(context_.renderPass))
    {
        renderPassContexts_[context_.renderPass].cmdBuf = VulkanCmdBuffer(driverDevice_);
        renderPassContexts_[context_.renderPass].completeSemaphore = vk::createSemaphore(driverDevice_);
    }

    auto &passContext = renderPassContexts_.at(context_.renderPass);
    passContext.frameOfLastUse = frameNr_;
    
    context_.cmdBuffer = &passContext.cmdBuf;
    context_.cmdBuffer->begin(false);

    context_.cmdBuffer->beginRenderPass(*context_.renderPass, currentFrameBuffer,
        static_cast<u32>(dimensions.x()), static_cast<u32>(dimensions.y()));

    if (context_.camera->hasColorClearing())
    {
        const VkClearRect clearRect{{{0, 0}, {static_cast<u32>(dimensions.x()), static_cast<u32>(dimensions.y())}}, 0, 1};
        auto clearColor = context_.camera->clearColor();
        const VkClearValue clearValue{{clearColor.x(), clearColor.y(), clearColor.z(), clearColor.w()}};
        const auto clearCount = renderTarget
            ? dynamic_cast<VulkanFrameBuffer*>(renderTarget)->colorAttachmentCount()
            : 1;
        for (u32 i = 0; i < clearCount; i++)
            context_.cmdBuffer->clearColorAttachment(i, clearValue, clearRect);
    }

    const auto viewport = context_.camera->viewport();
    context_.cmdBuffer->setViewport(viewport, 0, 1);
    context_.cmdBuffer->setScissor(viewport);
}

void VulkanRenderer::endCamera(Camera *)
{
	auto &ctx = renderPassContexts_.at(context_.renderPass);
    ctx.cmdBuf.endRenderPass();
    ctx.cmdBuf.end();

    vk::queueSubmit(driverDevice_.queue(), 1, &context_.waitSemaphore, 1, &ctx.completeSemaphore, 1, ctx.cmdBuf);
    vk::assertResult(vkQueueWaitIdle(driverDevice_.queue()));

    context_.waitSemaphore = ctx.completeSemaphore;

    context_.camera = nullptr;
}

void VulkanRenderer::renderMesh(Mesh *mesh, Transform *transform, Material *material)
{
	material = material ? material : errorMaterial();
    const auto vkMesh = dynamic_cast<VulkanMesh*>(mesh);
	
    bindPipelineAndMesh(material, transform, mesh);
	
    context_.cmdBuffer->draw(vkMesh->minVertexCount(), 1, 0, 0);
}

void VulkanRenderer::renderMeshIndex(Mesh *mesh, u32 index, Transform *transform, Material *material)
{
	material = material ? material : errorMaterial();
    const auto vkMesh = dynamic_cast<VulkanMesh*>(mesh);
	
    bindPipelineAndMesh(material, transform, mesh);
	
    const auto indexBuffer = vkMesh->indexBuffer(index);
	const auto indexType = toIndexType(mesh->indexBufferElementSize(index));
    context_.cmdBuffer->bindIndexBuffer(indexBuffer, 0, indexType);
    context_.cmdBuffer->drawIndexed(vkMesh->indexBufferElementCount(index), 1, 0, 0, 0);
}

void VulkanRenderer::renderDebugInterface(DebugInterface *debugInterface)
{
	context_.debugInterface.instance = dynamic_cast<VulkanDebugInterface*>(debugInterface);
}

void VulkanRenderer::bindPipelineAndMesh(Material *material, Transform *transform, Mesh *mesh)
{
    const auto vkMaterial = dynamic_cast<VulkanMaterial*>(material);
    const auto vkMesh = dynamic_cast<VulkanMesh*>(mesh);

	const auto key = contextKey(transform, context_.camera, vkMaterial, context_.renderPass->handle());
	if (!pipelineContexts_.count(key))
		pipelineContexts_.emplace(std::make_pair(key, VulkanPipelineContext(&driverDevice_, key)));

	auto &context = pipelineContexts_.at(key);
	context.setFrameOfLastUse(frameNr_);
    
    if (context_.pipelineContextKey != context.key())
    {
    	context.update(vkMaterial, vkMesh, context_.renderPass, context_.camera, transform);
        context_.cmdBuffer->bindPipeline(context.pipeline());
        context_.cmdBuffer->bindDescriptorSet(context.pipeline().layout(), context.descriptorSet());
        context_.pipelineContextKey = context.key();
    }

    // TODO don't rebind an already bound mesh (for instance when we render mesh indexes)
    for (u32 i = 0; i < vkMesh->vertexBufferCount(); i++)
        context_.cmdBuffer->bindVertexBuffer(i, vkMesh->vertexBuffer(i));
}

void VulkanRenderer::beginFrame()
{
    frameNr_++;
    context_.camera = nullptr;
    context_.renderPass = nullptr;
    context_.cmdBuffer = nullptr;
    context_.pipelineContextKey = 0;
	context_.debugInterface.instance = nullptr;
    context_.waitSemaphore = swapchain_.moveNext();
}

void VulkanRenderer::endFrame()
{
	// TODO extract function
	if (context_.debugInterface.instance)
	{
		context_.debugInterface.renderCmdBuffer.begin(false);

		const auto canvasSize = device_->canvasSize();
		
		context_.debugInterface.renderCmdBuffer.beginRenderPass(
			context_.debugInterface.instance->renderPass(),
			swapchain_.currentFrameBuffer(),
			canvasSize.x(), canvasSize.y());
		
		const auto viewport = Vector4(0, 0, canvasSize.x(), canvasSize.y());
		context_.debugInterface.renderCmdBuffer.setViewport(viewport, 0, 1);
		context_.debugInterface.renderCmdBuffer.setScissor(viewport);

		context_.debugInterface.instance->renderInto(context_.debugInterface.renderCmdBuffer);

		context_.debugInterface.renderCmdBuffer.endRenderPass();
		context_.debugInterface.renderCmdBuffer.end();

		vk::queueSubmit(driverDevice_.queue(), 1, &context_.waitSemaphore, 1, &context_.debugInterface.completeSemaphore, 1, context_.debugInterface.renderCmdBuffer);
	    vk::assertResult(vkQueueWaitIdle(driverDevice_.queue()));

		context_.waitSemaphore = context_.debugInterface.completeSemaphore;
	}
	
	swapchain_.present(driverDevice_.queue(), 1, &context_.waitSemaphore);
    vk::assertResult(vkQueueWaitIdle(driverDevice_.queue()));

    // TODO Naive cleanup, need better
    if (frameNr_ % 100 == 0)
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
	        if (frameNr_ - p.second.frameOfLastUse >= 100)
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
	        if (frameNr_ - p.second.frameOfLastUse() >= 100)
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
