/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloVulkanRenderPass.h"

#ifdef SL_VULKAN_RENDERER

#include <array>

using namespace solo;


RenderPass::RenderPass()
{
}


RenderPass::RenderPass(VkDevice device, vk::Resource<VkRenderPass> pass):
    device(device),
    pass(std::move(pass))
{
}


RenderPass::RenderPass(RenderPass &&other) noexcept
{
    swap(other);
}


auto RenderPass::operator=(RenderPass other) noexcept -> RenderPass&
{
    if (&other != this)
        swap(other);
    return *this;
}


void RenderPass::begin(VkCommandBuffer cmdBuf, VkFramebuffer framebuffer, uint32_t canvasWidth, uint32_t canvasHeight)
{
    VkRenderPassBeginInfo info{};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.pNext = nullptr;
    info.renderPass = pass;
	info.renderArea.offset.x = 0;
	info.renderArea.offset.y = 0;
	info.renderArea.extent.width = canvasWidth;
	info.renderArea.extent.height = canvasHeight;
	info.clearValueCount = clearValues.size();
	info.pClearValues = clearValues.data();
    info.framebuffer = framebuffer;

    vkCmdBeginRenderPass(cmdBuf, &info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdSetViewport(cmdBuf, 0, 1, &viewport);
    vkCmdSetScissor(cmdBuf, 0, 1, &scissor);
}


void RenderPass::end(VkCommandBuffer cmdBuf)
{
    vkCmdEndRenderPass(cmdBuf);
}


void RenderPass::swap(RenderPass& other) noexcept
{
    std::swap(device, other.device);
    std::swap(pass, other.pass);
}


RenderPassBuilder::RenderPassBuilder(VkDevice device):
    device(device)
{
}


auto RenderPassBuilder::withColorAttachment(VkFormat colorFormat) -> RenderPassBuilder&
{
    VkAttachmentDescription desc{};
    desc.format = colorFormat;
    desc.flags = 0;
    desc.samples = VK_SAMPLE_COUNT_1_BIT;
    desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments.push_back(desc);

    VkAttachmentReference reference{};
    reference.attachment = static_cast<uint32_t>(attachments.size() - 1);
    reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachmentRefs.push_back(reference);

    return *this;
}


auto RenderPassBuilder::withDepthAttachment(VkFormat depthFormat) -> RenderPassBuilder&
{
    VkAttachmentDescription desc{};
    desc.format = depthFormat;
    desc.flags = 0;
    desc.samples = VK_SAMPLE_COUNT_1_BIT;
    desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    desc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments.push_back(desc);

    depthAttachmentRef.attachment = static_cast<uint32_t>(attachments.size() - 1);
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    return *this;
}


auto RenderPassBuilder::build() -> RenderPass
{
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = colorAttachmentRefs.data();
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;

	std::array<VkSubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.flags = 0;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    vk::Resource<VkRenderPass> pass{device, vkDestroyRenderPass};
    SL_VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, pass.replace()));

    return RenderPass(device, std::move(pass));
}


void RenderPass::setViewport(uint32_t left, uint32_t top, uint32_t width, uint32_t height)
{
    viewport.x = static_cast<float>(left);
    viewport.y = static_cast<float>(top);
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
}


void RenderPass::setScissor(uint32_t left, uint32_t top, uint32_t width, uint32_t height)
{
    scissor.offset.x = left;
    scissor.offset.y = top;
    scissor.extent.width = width;
    scissor.extent.height = height;
}


void RenderPass::setClear(VkClearColorValue colorClear, VkClearDepthStencilValue depthStencilClear, bool clearColor, bool clearDepthStencil)
{
    clearValues.clear();

    if (clearColor)
    {
        VkClearValue colorValue;
        colorValue.color = colorClear;
        clearValues.push_back(colorValue);
    }

    if (clearDepthStencil)
    {
        VkClearValue depthValue;
        depthValue.depthStencil = depthStencilClear;
        clearValues.push_back(depthValue);
    }
}


#endif
