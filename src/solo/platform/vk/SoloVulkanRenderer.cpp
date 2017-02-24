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

#include "SoloDevice.h"
#include "SoloSDLVulkanDevice.h"
#include "SoloVulkanCamera.h"

using namespace solo;
using namespace vk;


vk::Renderer::Renderer(Device *engineDevice):
    engineDevice(engineDevice)
{
    auto vulkanDevice = dynamic_cast<SDLDevice*>(engineDevice);
    auto instance = vulkanDevice->getInstance();
    auto surface = vulkanDevice->getSurface();
    auto canvasWidth = engineDevice->getSetup().canvasWidth;
    auto canvasHeight = engineDevice->getSetup().canvasHeight;

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

    renderCmdBuffers.resize(swapchain.getStepCount());
    createCommandBuffers(this->device, commandPool, swapchain.getStepCount(), renderCmdBuffers.data());
}


vk::Renderer::~Renderer()
{
    vkFreeCommandBuffers(device, commandPool, renderCmdBuffers.size(), renderCmdBuffers.data());
}


void vk::Renderer::beginFrame()
{
    currentBuffer = swapchain.getNextImageIndex(semaphores.presentComplete);
    prevRenderCommands = std::move(renderCommands);
    renderCommands = std::vector<RenderCommand>(100); // TODO just picked random constant
}


void vk::Renderer::endFrame()
{
    if (dirty)
    {
        recordRenderCmdBuffers();
//        dirty = false; TODO
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
	submitInfo.pCommandBuffers = &renderCmdBuffers[currentBuffer];
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


void vk::Renderer::applyRenderCommands(VkCommandBuffer buf, VkFramebuffer framebuffer)
{
    for (const auto &cmd: renderCommands)
    {
        switch (cmd.type)
        {
            case RenderCommandType::BeginCamera:
            {
                auto color = cmd.camera->getClearColor();
                renderPass.setClear(cmd.camera->isClearColorEnabled(), cmd.camera->isClearDepthEnabled(),
                    {{color.x, color.y, color.z, color.w}},
                    {1, 0});
                    
                auto viewport = cmd.camera->getViewport();
                auto vp = VkViewport{viewport.x, viewport.y, viewport.z, viewport.w, 1, 100};
                
                renderPass.begin(buf, framebuffer, vp.width, vp.height);
                vkCmdSetViewport(buf, 0, 1, &vp);

                // TODO this is temp
                VkRect2D scissor;
                scissor.offset.x = 0;
                scissor.offset.y = 0;
                scissor.extent.width = vp.width;
                scissor.extent.height = vp.height;
                vkCmdSetScissor(buf, 0, 1, &scissor);

                break;
            }

            case RenderCommandType::EndCamera:
                renderPass.end(buf);
                break;

            default:
                break;
        }
    }
}


void vk::Renderer::recordRenderCmdBuffers()
{
    for (size_t i = 0; i < renderCmdBuffers.size(); i++)
    {
        auto buf = renderCmdBuffers[i];

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        SL_VK_CHECK_RESULT(vkBeginCommandBuffer(buf, &beginInfo));

        applyRenderCommands(buf, swapchain.getFramebuffer(i));

        SL_VK_CHECK_RESULT(vkEndCommandBuffer(buf));
    }
}


void vk::Renderer::beginCamera(const Camera *camera)
{
    renderCommands.push_back(RenderCommand::beginCamera(camera));
}


void vk::Renderer::endCamera()
{
    renderCommands.push_back(RenderCommand::endCamera());
}


void vk::Renderer::drawMesh(const Mesh *mesh)
{
    renderCommands.push_back(RenderCommand::drawMesh(mesh));
}


#endif
