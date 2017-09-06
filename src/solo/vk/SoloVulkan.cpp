/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkan.h"

#ifdef SL_VULKAN_RENDERER

#include <vector>
#include <array>

using namespace solo;

auto vk::createSemaphore(VkDevice device) -> Resource<VkSemaphore>
{
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    Resource<VkSemaphore> semaphore{device, vkDestroySemaphore};
    SL_VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, semaphore.cleanRef()));

    return semaphore;
}

auto vk::createCommandBuffer(VkDevice device, VkCommandPool commandPool) -> Resource<VkCommandBuffer>
{
    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.pNext = nullptr;
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    Resource<VkCommandBuffer> buffer{device, commandPool, vkFreeCommandBuffers};
    SL_VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocateInfo, &buffer));

    return buffer;
}

void vk::beginCommandBuffer(VkCommandBuffer buffer, bool oneTime)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = oneTime ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : 0;
    SL_VK_CHECK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo));
}

void vk::queueSubmit(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore *waitSemaphores,
    uint32_t signalSemaphoreCount, const VkSemaphore *signalSemaphores,
    uint32_t commandBufferCount, const VkCommandBuffer *commandBuffers)
{
    VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
    submitInfo.waitSemaphoreCount = waitSemaphoreCount;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.signalSemaphoreCount = signalSemaphoreCount;
    submitInfo.pSignalSemaphores = signalSemaphores;
    submitInfo.commandBufferCount = commandBufferCount;
    submitInfo.pCommandBuffers = commandBuffers;
    SL_VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
}

auto vk::findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, uint32_t typeBits,
    VkMemoryPropertyFlags properties) -> int32_t
{
    for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((typeBits & 1) == 1)
        {
            if ((physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }
        typeBits >>= 1;
    }
    return -1;
}

auto vk::createFrameBuffer(VkDevice device, VkImageView colorAttachment, VkImageView depthAttachment,
    VkRenderPass renderPass, uint32_t width, uint32_t height) -> Resource<VkFramebuffer>
{
    std::array<VkImageView, 2> attachments = {colorAttachment, depthAttachment};

    VkFramebufferCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.renderPass = renderPass;
    createInfo.attachmentCount = attachments.size();
    createInfo.pAttachments = attachments.data();
    createInfo.width = width;
    createInfo.height = height;
    createInfo.layers = 1;

    Resource<VkFramebuffer> frameBuffer{device, vkDestroyFramebuffer};
    SL_VK_CHECK_RESULT(vkCreateFramebuffer(device, &createInfo, nullptr, frameBuffer.cleanRef()));

    return frameBuffer;
}

auto vk::createShader(VkDevice device, const void *data, uint32_t size) -> Resource<VkShaderModule>
{
    VkShaderModuleCreateInfo shaderModuleInfo {};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.pNext = nullptr;
    shaderModuleInfo.flags = 0;
    shaderModuleInfo.codeSize = size;
    shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(data);

    Resource<VkShaderModule> module{device, vkDestroyShaderModule};
    SL_VK_CHECK_RESULT(vkCreateShaderModule(device, &shaderModuleInfo, nullptr, module.cleanRef()));

    return module;
}

auto vk::createImageView(VkDevice device, VkFormat format, VkImageViewType type, uint32_t mipLevels, uint32_t layers,
    VkImage image, VkImageAspectFlags aspectMask) -> Resource<VkImageView>
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.viewType = type;
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.format = format;
    viewInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
    viewInfo.subresourceRange = {};
    viewInfo.subresourceRange.aspectMask = aspectMask;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = layers;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.image = image;

    Resource<VkImageView> view{device, vkDestroyImageView};
    SL_VK_CHECK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, view.cleanRef()));

    return view;
}

auto vk::createShaderStageInfo(bool vertex, VkShaderModule shader, const char* entryPoint) -> VkPipelineShaderStageCreateInfo
{
    VkPipelineShaderStageCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.stage = vertex ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
    info.module = shader;
    info.pName = entryPoint;
    info.pSpecializationInfo = nullptr;
    return info;
}

#endif
