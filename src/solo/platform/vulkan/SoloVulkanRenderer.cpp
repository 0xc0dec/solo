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

#ifdef SL_VULKAN_RENDERER

#include "SoloSDLVulkanDevice.h"
#include "SoloVulkanSwapchain.h"

using namespace solo;


VulkanRenderer::VulkanRenderer(Device *engineDevice):
    canvasWidth(engineDevice->getSetup().canvasWidth),
    canvasHeight(engineDevice->getSetup().canvasHeight)
{
    auto vulkanDevice = dynamic_cast<SDLVulkanDevice*>(engineDevice);
    auto instance = vulkanDevice->getVkInstance();
    auto surface = vulkanDevice->getVkSurface();

    physicalDevice.device = vk::getPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice.device, &physicalDevice.properties);
    vkGetPhysicalDeviceFeatures(physicalDevice.device, &physicalDevice.features);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice.device, &physicalDevice.memProperties);

    auto surfaceFormats = vk::getSurfaceFormats(physicalDevice.device, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = vk::getQueueIndex(physicalDevice.device, surface);

    device = vk::createDevice(physicalDevice.device, queueIndex);

    vkGetDeviceQueue(device, queueIndex, 0, &queue);

    depthFormat = vk::getDepthFormat(physicalDevice.device);
    semaphores.presentComplete = vk::createSemaphore(device);
    semaphores.renderComplete = vk::createSemaphore(device);
    commandPool = vk::createCommandPool(device, queueIndex);
    depthStencil = vk::createDepthStencil(device, physicalDevice.memProperties, depthFormat, canvasWidth, canvasHeight);
    renderPass = vk::createRenderPass(device, colorFormat, depthFormat);
    
    swapchain = std::make_shared<VulkanSwapchain>(device, physicalDevice.device, surface, renderPass, depthStencil.view,
        this->canvasWidth, this->canvasHeight, engineDevice->getSetup().vsync, colorFormat, colorSpace);
    
    cmdBuffers.resize(swapchain->getSegmentCount());
    vk::createCommandBuffers(device, commandPool, swapchain->getSegmentCount(), cmdBuffers.data());
}


VulkanRenderer::~VulkanRenderer()
{
    vkFreeCommandBuffers(device, commandPool, cmdBuffers.size(), cmdBuffers.data());

    swapchain.reset();

    // Render pass
    vkDestroyRenderPass(device, renderPass, nullptr);

    // Depth stencil
    if (depthStencil.view)
        vkDestroyImageView(device, depthStencil.view, nullptr);
    if (depthStencil.image)
        vkDestroyImage(device, depthStencil.image, nullptr);
    if (depthStencil.mem)
        vkFreeMemory(device, depthStencil.mem, nullptr);

    // Command pool
    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroySemaphore(device, semaphores.presentComplete, nullptr);
    vkDestroySemaphore(device, semaphores.renderComplete, nullptr);

    vkDestroyDevice(device, nullptr);
}


void VulkanRenderer::beginFrame()
{
    currentBuffer = swapchain->getNextImageIndex(semaphores.presentComplete);
}


void VulkanRenderer::endFrame()
{
    if (dirty)
    {
        updateCmdBuffers();
        dirty = false;
    }

    VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &semaphores.presentComplete;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &semaphores.renderComplete;
    submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuffers[currentBuffer];
    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

    VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain->getHandle();
	presentInfo.pImageIndices = &currentBuffer;
	presentInfo.pWaitSemaphores = &semaphores.renderComplete;
	presentInfo.waitSemaphoreCount = 1;
    SL_CHECK_VK_RESULT(vkQueuePresentKHR(queue, &presentInfo));

    SL_CHECK_VK_RESULT(vkQueueWaitIdle(queue));
}


void VulkanRenderer::updateCmdBuffers()
{
    VkRect2D scissor{};
    scissor.extent.width = canvasWidth - 50;
    scissor.extent.height = canvasHeight - 50;
    scissor.offset.x = 50;
    scissor.offset.y = 50;

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = canvasWidth;
	renderPassBeginInfo.renderArea.extent.height = canvasHeight;
	renderPassBeginInfo.clearValueCount = clearValues.size();
	renderPassBeginInfo.pClearValues = clearValues.data();

    for (size_t i = 0; i < cmdBuffers.size(); i++)
    {
        renderPassBeginInfo.framebuffer = swapchain->getFramebuffer(i);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        SL_CHECK_VK_RESULT(vkBeginCommandBuffer(cmdBuffers[i], &beginInfo));

        vkCmdBeginRenderPass(cmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdSetViewport(cmdBuffers[i], 0, 1, &viewport);
        vkCmdSetScissor(cmdBuffers[i], 0, 1, &scissor);

        vkCmdEndRenderPass(cmdBuffers[i]);

        SL_CHECK_VK_RESULT(vkEndCommandBuffer(cmdBuffers[i]));
    }
}


void VulkanRenderer::setClear(const Vector4& color, bool clearColor, bool clearDepth)
{
    clearValues.clear();

    if (clearColor)
    {
        VkClearValue colorValue;
        colorValue.color = {color.x, color.y, color.z, color.w};
        clearValues.push_back(colorValue);
    }

    if (clearDepth)
    {
        VkClearValue depthValue;
        depthValue.depthStencil = {1, 0};
        clearValues.push_back(depthValue);
    }

    dirty = true;
}


void VulkanRenderer::setViewport(const Vector4& viewport)
{
	this->viewport.x = viewport.x;
	this->viewport.y = viewport.y;
	this->viewport.width = viewport.z;
	this->viewport.height = viewport.w;
	this->viewport.minDepth = 0;
	this->viewport.maxDepth = 1;
    dirty = true;
}


#endif
