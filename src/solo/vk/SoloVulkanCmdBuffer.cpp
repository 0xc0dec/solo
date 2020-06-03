/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanCmdBuffer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanRenderPass.h"
#include "SoloVulkanRenderer.h"

using namespace solo;

VulkanCmdBuffer::VulkanCmdBuffer(const VulkanDriverDevice &dev):
    device_(&dev)
{
    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.pNext = nullptr;
    allocateInfo.commandPool = dev.commandPool();
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    handle_ = VulkanResource<VkCommandBuffer>{dev.handle(), dev.commandPool(), vkFreeCommandBuffers};
    vk::assertResult(vkAllocateCommandBuffers(dev.handle(), &allocateInfo, &handle_));
}

auto VulkanCmdBuffer::beginRenderPass(const VulkanRenderPass &pass, VkFramebuffer framebuffer, u32 canvasWidth,
    u32 canvasHeight) -> VulkanCmdBuffer&
{
    VkRenderPassBeginInfo info{};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.pNext = nullptr;
    info.renderPass = pass;
    info.renderArea.offset.x = 0;
    info.renderArea.offset.y = 0;
    info.renderArea.extent.width = canvasWidth;
    info.renderArea.extent.height = canvasHeight;
    info.clearValueCount = pass.clearValues().size();
    info.pClearValues = pass.clearValues().data();
    info.framebuffer = framebuffer;

    vkCmdBeginRenderPass(handle_, &info, VK_SUBPASS_CONTENTS_INLINE);

    return *this;
}

auto VulkanCmdBuffer::endRenderPass() -> VulkanCmdBuffer&
{
    vkCmdEndRenderPass(handle_);
    return *this;
}

auto VulkanCmdBuffer::bindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
    -> VulkanCmdBuffer&
{
    vkCmdBindIndexBuffer(handle_, buffer, offset, indexType);
    return *this;
}

auto VulkanCmdBuffer::bindVertexBuffer(u32 binding, VkBuffer buffer) -> VulkanCmdBuffer&
{
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(handle_, binding, 1, &buffer, &offset);
    return *this;
}

auto VulkanCmdBuffer::drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex,
    u32 vertexOffset, u32 firstInstance) -> VulkanCmdBuffer&
{
    vkCmdDrawIndexed(handle_, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    return *this;
}

auto VulkanCmdBuffer::draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance)
    -> VulkanCmdBuffer&
{
    vkCmdDraw(handle_, vertexCount, instanceCount, firstVertex, firstInstance);
    return *this;
}

auto VulkanCmdBuffer::bindPipeline(VkPipeline pipeline) -> VulkanCmdBuffer&
{
    vkCmdBindPipeline(handle_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    return *this;
}

auto VulkanCmdBuffer::bindDescriptorSet(VkPipelineLayout pipelineLayout, const VulkanDescriptorSet &set)
    -> VulkanCmdBuffer&
{
    vkCmdBindDescriptorSets(handle_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, set, 0, nullptr);
    return *this;
}

auto VulkanCmdBuffer::setViewport(const Vector4 &dimentions, float minDepth, float maxDepth) -> VulkanCmdBuffer&
{
    VkViewport vp{dimentions.x(), dimentions.y(), dimentions.z(), dimentions.w(), minDepth, maxDepth};
    vkCmdSetViewport(handle_, 0, 1, &vp);
    return *this;
}

auto VulkanCmdBuffer::setScissor(const Vector4 &dimentions) -> VulkanCmdBuffer&
{
    VkRect2D scissor{{0, 0}, {static_cast<u32>(dimentions.z()), static_cast<u32>(dimentions.w())}};
    // TODO proper values
    vkCmdSetScissor(handle_, 0, 1, &scissor);
    return *this;
}

auto VulkanCmdBuffer::putImagePipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const VkImageMemoryBarrier &barrier) -> VulkanCmdBuffer&
{
    vkCmdPipelineBarrier(handle_, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    return *this;
}

auto VulkanCmdBuffer::clearColorAttachment(u32 attachment, const VkClearValue &clearValue, const VkClearRect &clearRect)
    -> VulkanCmdBuffer&
{
    VkClearAttachment clear;
    clear.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    clear.colorAttachment = attachment;
    clear.clearValue = clearValue;
    vkCmdClearAttachments(handle_, 1, &clear, 1, &clearRect); // TODO in batch
    return *this;
}

auto VulkanCmdBuffer::copyBuffer(const VulkanBuffer &src, const VulkanBuffer &dst) -> VulkanCmdBuffer&
{
    VkBufferCopy copyRegion{};
    copyRegion.size = dst.size();
    vkCmdCopyBuffer(handle_, src.handle(), dst.handle(), 1, &copyRegion);
    return *this;
}

auto VulkanCmdBuffer::copyBuffer(const VulkanBuffer &src, const VulkanImage &dst) -> VulkanCmdBuffer&
{
    VkBufferImageCopy bufferCopyRegion{};
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferCopyRegion.imageSubresource.mipLevel = 0;
    bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
    bufferCopyRegion.imageSubresource.layerCount = 1;
    bufferCopyRegion.imageExtent.width = dst.width();
    bufferCopyRegion.imageExtent.height = dst.height();
    bufferCopyRegion.imageExtent.depth = 1;
    bufferCopyRegion.bufferOffset = 0;

    vkCmdCopyBufferToImage(
        handle_,
        src.handle(),
        dst.handle(),
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &bufferCopyRegion);

    return *this;
}

auto VulkanCmdBuffer::copyBuffer(const VulkanBuffer &src, const VulkanImage &dst, const VkBufferImageCopy *regions,
    u32 regionCount) -> VulkanCmdBuffer&
{
    vkCmdCopyBufferToImage(
        handle_,
        src.handle(),
        dst.handle(),
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        regionCount,
        regions);
    return *this;
}

auto VulkanCmdBuffer::blit(VkImage src, VkImage dst, VkImageLayout srcLayout, VkImageLayout dstLayout,
    const VkImageBlit &blit, VkFilter filter) -> VulkanCmdBuffer&
{
    vkCmdBlitImage(handle_,
        src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, &blit,
        filter);
    return *this;
}

void VulkanCmdBuffer::endAndFlush()
{
    end();
    vk::queueSubmit(device_->queue(), 0, nullptr, 0, nullptr, 1, &handle_);
    vk::assertResult(vkQueueWaitIdle(device_->queue()));
}

auto VulkanCmdBuffer::begin(bool transient) -> VulkanCmdBuffer&
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = transient ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : 0;
    vk::assertResult(vkBeginCommandBuffer(handle_, &beginInfo));
    return *this;
}

void VulkanCmdBuffer::end() const
{
    vk::assertResult(vkEndCommandBuffer(handle_));
}

#endif
