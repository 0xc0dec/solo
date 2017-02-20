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
#include "SoloVulkan.h"

using namespace solo;
using namespace vk;


vk::Renderer::Renderer(Device *engineDevice):
    canvasWidth(engineDevice->getSetup().canvasWidth),
    canvasHeight(engineDevice->getSetup().canvasHeight)
{
    auto vulkanDevice = dynamic_cast<SDLDevice*>(engineDevice);
    auto instance = vulkanDevice->getVkInstance();
    auto surface = vulkanDevice->getVkSurface();

    physicalDevice.device = getPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice.device, &physicalDevice.properties);
    vkGetPhysicalDeviceFeatures(physicalDevice.device, &physicalDevice.features);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice.device, &physicalDevice.memProperties);

    auto surfaceFormats = getSurfaceFormats(physicalDevice.device, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = getQueueIndex(physicalDevice.device, surface);
    this->device = createDevice(physicalDevice.device, queueIndex);

    vkGetDeviceQueue(this->device, queueIndex, 0, &queue);

    depthFormat = getDepthFormat(physicalDevice.device);
    commandPool = createCommandPool(this->device, queueIndex);
    depthStencil = createDepthStencil(this->device, physicalDevice.memProperties, depthFormat, canvasWidth, canvasHeight);
    renderPass = RenderPassBuilder(this->device)
        .withColorAttachment(colorFormat)
        .withDepthAttachment(depthFormat)
        .build();

    swapchain = Swapchain(this->device, physicalDevice.device, surface, renderPass, depthStencil.view,
        canvasWidth, canvasHeight, false, colorFormat, colorSpace);

    semaphores.presentComplete = createSemaphore(this->device);
    semaphores.renderComplete = createSemaphore(this->device);

    cmdBuffers.resize(swapchain.getStepCount());
    createCommandBuffers(this->device, commandPool, swapchain.getStepCount(), cmdBuffers.data());
}


vk::Renderer::~Renderer()
{
    vkFreeCommandBuffers(device, commandPool, cmdBuffers.size(), cmdBuffers.data());
}


void vk::Renderer::beginFrame()
{
    currentBuffer = swapchain.getNextImageIndex(semaphores.presentComplete);
}


void vk::Renderer::endFrame()
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
    SL_VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

    auto swapchainHandle = swapchain.getHandle();
    VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchainHandle;
	presentInfo.pImageIndices = &currentBuffer;
	presentInfo.pWaitSemaphores = &semaphores.renderComplete;
	presentInfo.waitSemaphoreCount = 1;
    SL_VK_CHECK_RESULT(vkQueuePresentKHR(queue, &presentInfo));

    SL_VK_CHECK_RESULT(vkQueueWaitIdle(queue));
}


void vk::Renderer::updateCmdBuffers()
{
    renderPass.setViewport(0, 0, canvasWidth, canvasHeight);
    renderPass.setScissor(0, 0, canvasWidth, canvasHeight);

    for (size_t i = 0; i < cmdBuffers.size(); i++)
    {
        auto buf = cmdBuffers[i];

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        SL_VK_CHECK_RESULT(vkBeginCommandBuffer(buf, &beginInfo));

        renderPass.begin(buf, swapchain.getFramebuffer(i), canvasWidth, canvasHeight);
        renderPass.end(buf);

        SL_VK_CHECK_RESULT(vkEndCommandBuffer(cmdBuffers[i]));
    }
}


void vk::Renderer::setClear(const Vector4& color, bool clearColor, bool clearDepth)
{
    renderPass.setClear({color.x, color.y, color.z, color.w}, {1, 0}, clearColor, clearDepth);
    dirty = true;
}


void vk::Renderer::setViewport(const Vector4& viewport)
{
    renderPass.setViewport(viewport.x, viewport.y, viewport.z, viewport.w);
    renderPass.setScissor(0, 0, canvasWidth, canvasHeight); // TODO this is temp
    dirty = true;
}


#endif
