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

#include "SoloVulkanRenderer.h"
#include "SoloSDLVulkanDevice.h"
#include "SoloVulkanPipeline.h"
#include "SoloVulkanSwapchain.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


static auto createFrameBuffers(VkDevice device, VulkanSwapchain *swapchain, VkImageView depthStencilView,
    VkRenderPass renderPass, uint32_t width, uint32_t height) -> std::vector<VkFramebuffer>
{
    auto count = swapchain->getImageCount();
    std::vector<VkFramebuffer> buffers(count);
    for (auto i = 0; i < count; i++)
        buffers[i] = vk::createFrameBuffer(device, swapchain->getImageView(i), depthStencilView, renderPass, width, height);
    return std::move(buffers);
}


static auto createRenderCmdBuffers(uint32_t count, VkDevice device, VkCommandPool commandPool) -> std::vector<VkCommandBuffer>
{
    std::vector<VkCommandBuffer> buffers(count);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(count);

    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, buffers.data()));

    return std::move(buffers);
}


// TODO this is only for testing
static VulkanPipeline *pipeline = nullptr;


VulkanRenderer::VulkanRenderer(Device *engineDevice):
    canvasWidth(engineDevice->getSetup().canvasWidth),
    canvasHeight(engineDevice->getSetup().canvasHeight)
{
    auto vulkanDevice = dynamic_cast<SDLVulkanDevice*>(engineDevice);
    auto instance = vulkanDevice->getVkInstance();
    auto surface = vulkanDevice->getVkSurface();

    physicalDevice = vk::getPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &features);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    auto surfaceFormats = vk::getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = vk::getQueueIndex(physicalDevice, surface);

    device = vk::createDevice(physicalDevice, queueIndex);

    vkGetDeviceQueue(device, queueIndex, 0, &queue);

    depthFormat = vk::getDepthFormat(physicalDevice);
    presentCompleteSem = vk::createSemaphore(device);
    renderCompleteSem = vk::createSemaphore(device);
    commandPool = vk::createCommandPool(device, queueIndex);
    swapchain = std::make_shared<VulkanSwapchain>(device, physicalDevice, surface, engineDevice->getSetup().vsync,
        Vector2(canvasWidth, canvasHeight), colorFormat, colorSpace);
    depthStencil = vk::createDepthStencil(device, memProperties, depthFormat, canvasWidth, canvasHeight);
    renderPass = vk::createRenderPass(device, colorFormat, depthFormat);

    frameBuffers = createFrameBuffers(device, swapchain.get(), depthStencil.view, renderPass, canvasWidth, canvasHeight);
    renderCmdBuffers = createRenderCmdBuffers(swapchain->getImageCount(), device, commandPool);

    // TODO remove
    buildCommandBuffers();
}


VulkanRenderer::~VulkanRenderer()
{
    vkFreeCommandBuffers(device, commandPool, renderCmdBuffers.size(), renderCmdBuffers.data());

    for (size_t i = 0; i < frameBuffers.size(); ++i)
        vkDestroyFramebuffer(device, frameBuffers[i], nullptr);

    // Render pass
    vkDestroyRenderPass(device, renderPass, nullptr);

    // Depth stencil
    if (depthStencil.view)
        vkDestroyImageView(device, depthStencil.view, nullptr);
    if (depthStencil.image)
        vkDestroyImage(device, depthStencil.image, nullptr);
    if (depthStencil.mem)
        vkFreeMemory(device, depthStencil.mem, nullptr);

    // Swapchain
    swapchain.reset();

    // Command pool
    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroySemaphore(device, presentCompleteSem, nullptr);
    vkDestroySemaphore(device, renderCompleteSem, nullptr);

    vkDestroyDevice(device, nullptr);
}


void VulkanRenderer::beginFrame()
{
    currentBuffer = swapchain->acquireNextImage(presentCompleteSem);
}


void VulkanRenderer::endFrame()
{
    VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &presentCompleteSem;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderCompleteSem;
    submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &renderCmdBuffers[currentBuffer];

    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

    swapchain->present(queue, currentBuffer, renderCompleteSem);

    SL_CHECK_VK_RESULT(vkQueueWaitIdle(queue));
}


void VulkanRenderer::buildCommandBuffers()
{
    static VkClearColorValue defaultClearColor = {{0.5f, 0.1f, 0.1f, 1.0f}};

    // Build

    VkViewport viewport {};
	viewport.width = canvasWidth;
	viewport.height = canvasHeight;
	viewport.minDepth = 0;
	viewport.maxDepth = 1;

    VkRect2D scissor {};
    scissor.extent.width = canvasWidth;
    scissor.extent.height = canvasHeight;
    scissor.offset.x = 0;
    scissor.offset.y = 0;

    VkClearValue clearValues[2];
	clearValues[0].color = defaultClearColor;
    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = canvasWidth;
	renderPassBeginInfo.renderArea.extent.height = canvasHeight;
	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValues;

    // TODO this is only for testing
    for (size_t i = 0; i < renderCmdBuffers.size(); i++)
    {
        renderPassBeginInfo.framebuffer = frameBuffers[i];

        vk::recordCommandBuffer(renderCmdBuffers[i], [=](VkCommandBuffer buf)
        {
            vkCmdBeginRenderPass(buf, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdSetViewport(renderCmdBuffers[i], 0, 1, &viewport);
            vkCmdSetScissor(renderCmdBuffers[i], 0, 1, &scissor);

            vkCmdEndRenderPass(buf);
        });
    }
}

#endif
