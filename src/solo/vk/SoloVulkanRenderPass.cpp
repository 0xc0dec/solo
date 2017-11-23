/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanRenderPass.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;
using namespace vk;

VulkanRenderPass::VulkanRenderPass(VkDevice device, const VulkanRenderPassConfig &config):
    device(device),
    clearValues(config.clearValues)
{
    const auto colorAttachments = config.colorAttachmentRefs.empty() ? nullptr : config.colorAttachmentRefs.data();
    const auto depthAttachment = config.depthAttachmentRef.layout != VK_IMAGE_LAYOUT_UNDEFINED ? &config.depthAttachmentRef : nullptr;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = config.colorAttachmentRefs.size();
    subpass.pColorAttachments = colorAttachments;
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = depthAttachment;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;

	arr<VkSubpassDependency, 2> dependencies;

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
    renderPassInfo.attachmentCount = config.attachments.size();
    renderPassInfo.pAttachments = config.attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = dependencies.size();
    renderPassInfo.pDependencies = dependencies.data();

    VulkanResource<VkRenderPass> pass{device, vkDestroyRenderPass};
    SL_VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, pass.cleanRef()));

    this->pass = std::move(pass);
}

void VulkanRenderPass::begin(VkCommandBuffer cmdBuf, VkFramebuffer framebuffer, u32 canvasWidth, u32 canvasHeight)
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
}

void VulkanRenderPass::end(VkCommandBuffer cmdBuf)
{
    vkCmdEndRenderPass(cmdBuf);
}

VulkanRenderPassConfig::VulkanRenderPassConfig():
    depthAttachmentRef{0, VK_IMAGE_LAYOUT_UNDEFINED}
{
}

auto VulkanRenderPassConfig::withColorAttachment(VkFormat colorFormat, VkImageLayout finalLayout, bool clear, VkClearColorValue clearValue) -> VulkanRenderPassConfig&
{
    VkAttachmentDescription desc{};
    desc.format = colorFormat;
    desc.flags = 0;
    desc.samples = VK_SAMPLE_COUNT_1_BIT;
    desc.loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    desc.finalLayout = finalLayout;
    attachments.push_back(desc);

    VkAttachmentReference reference{};
    reference.attachment = attachments.size() - 1;
    reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachmentRefs.push_back(reference);

    clearValues.push_back({clearValue});

    return *this;
}

auto VulkanRenderPassConfig::withDepthAttachment(VkFormat depthFormat, bool clear, VkClearDepthStencilValue clearValue) -> VulkanRenderPassConfig&
{
    VkAttachmentDescription desc{};
    desc.format = depthFormat;
    desc.flags = 0;
    desc.samples = VK_SAMPLE_COUNT_1_BIT;
    desc.loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    desc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments.push_back(desc);

    depthAttachmentRef.attachment = attachments.size() - 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkClearValue cv;
    cv.depthStencil = clearValue;
    clearValues.push_back(cv);

    return *this;
}

#endif
