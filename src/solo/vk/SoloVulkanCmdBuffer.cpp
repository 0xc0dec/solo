/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanCmdBuffer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanRenderPass.h"

using namespace solo;

VulkanCmdBuffer::VulkanCmdBuffer(VkDevice device, VkCommandPool commandPool)
{
    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.pNext = nullptr;
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    VulkanResource<VkCommandBuffer> buffer{device, commandPool, vkFreeCommandBuffers};
    SL_VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocateInfo, &buffer));

    handle_ = std::move(buffer);
}

void VulkanCmdBuffer::beginRenderPass(const VulkanRenderPass &pass, VkFramebuffer framebuffer, u32 canvasWidth, u32 canvasHeight)
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
}

void VulkanCmdBuffer::endRenderPass()
{
    vkCmdEndRenderPass(handle_);
}

void VulkanCmdBuffer::bindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    vkCmdBindIndexBuffer(handle_, buffer, 0, indexType);
}

void VulkanCmdBuffer::bindVertexBuffer(u32 binding, VkBuffer buffer)
{
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(handle_, binding, 1, &buffer, &offset);
}

void VulkanCmdBuffer::drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex,
    u32 vertexOffset, u32 firstInstance)
{
    vkCmdDrawIndexed(handle_, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VulkanCmdBuffer::draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance)
{
    vkCmdDraw(handle_, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanCmdBuffer::bindPipeline(VkPipeline pipeline)
{
    vkCmdBindPipeline(handle_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void VulkanCmdBuffer::bindDescriptorSet(VkPipelineLayout pipelineLayout, const VulkanDescriptorSet &set)
{
    vkCmdBindDescriptorSets(handle_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, set, 0, nullptr);
}

void VulkanCmdBuffer::setViewport(const Vector4 &dimentions, float minDepth, float maxDepth)
{
    VkViewport vp{dimentions.x(), dimentions.y(), dimentions.z(), dimentions.w(), minDepth, maxDepth};
    vkCmdSetViewport(handle_, 0, 1, &vp);
}

void VulkanCmdBuffer::setScissor(const Vector4 &dimentions)
{
    VkRect2D scissor{{0, 0}, {static_cast<u32>(dimentions.z()), static_cast<u32>(dimentions.w())}}; // TODO proper values
    vkCmdSetScissor(handle_, 0, 1, &scissor);
}

void VulkanCmdBuffer::clearColorAttachment(u32 attachment, const VkClearValue &clearValue, const VkClearRect &clearRect)
{
    VkClearAttachment clear;
    clear.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    clear.colorAttachment = attachment;
    clear.clearValue = clearValue;
    vkCmdClearAttachments(handle_, 1, &clear, 1, &clearRect); // TODO in batch
}

void VulkanCmdBuffer::begin(bool oneTime)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = oneTime ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : 0;
    SL_VK_CHECK_RESULT(vkBeginCommandBuffer(handle_, &beginInfo));
}

void VulkanCmdBuffer::end()
{
    SL_VK_CHECK_RESULT(vkEndCommandBuffer(handle_));
}

#endif
