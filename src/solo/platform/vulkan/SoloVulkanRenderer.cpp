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
// TODO remove
#include "SoloFileSystem.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


auto VulkanRenderer::createDepthStencil(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps,
    VkFormat depthFormat, uint32_t canvasWidth, uint32_t canvasHeight) -> DepthStencil
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = nullptr;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = depthFormat;
    imageInfo.extent = {canvasWidth, canvasHeight, 1};
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    imageInfo.flags = 0;

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = 0;
    allocInfo.memoryTypeIndex = 0;

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.pNext = nullptr;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = depthFormat;
    viewInfo.flags = 0;
    viewInfo.subresourceRange = {};
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    DepthStencil depthStencil;

    VkMemoryRequirements memReqs;
    SL_CHECK_VK_RESULT(vkCreateImage(device, &imageInfo, nullptr, &depthStencil.image));
    vkGetImageMemoryRequirements(device, depthStencil.image, &memReqs);

    auto memTypeIndex = vk::findMemoryType(physicalDeviceMemProps, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    SL_ERR_IF(memTypeIndex < 0);

    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = memTypeIndex;
    SL_CHECK_VK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, &depthStencil.mem));
    SL_CHECK_VK_RESULT(vkBindImageMemory(device, depthStencil.image, depthStencil.mem, 0));

    viewInfo.image = depthStencil.image;
    SL_CHECK_VK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, &depthStencil.view));

    return depthStencil;
}


void VulkanRenderer::initFrameBuffers()
{
    auto count = swapchain->getImageCount();
    frameBuffers.resize(count);
    for (auto i = 0; i < count; i++)
        frameBuffers[i] = vk::createFrameBuffer(device, swapchain->getImageView(i), depthStencil.view, renderPass, canvasWidth, canvasHeight);
}


// TODO this is only for testing
static SDLVulkanDevice *vulkanDevice = nullptr;


VulkanRenderer::VulkanRenderer(Device *engineDevice)
{
    vulkanDevice = dynamic_cast<SDLVulkanDevice*>(engineDevice);

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
        engineDevice->getCanvasSize(), colorFormat, colorSpace);
    depthStencil = createDepthStencil(device, memProperties, depthFormat, canvasWidth, canvasHeight);
    renderPass = vk::createRenderPass(device, colorFormat, depthFormat);

    initFrameBuffers();
    initCommandBuffers();
}


// TODO this is only for testing
static VulkanPipeline *pipeline = nullptr;
static bool init = false;


VulkanRenderer::~VulkanRenderer()
{
    // TODO this is only for testing
    delete pipeline;

    swapchain.reset();

    if (presentCompleteSem)
        vkDestroySemaphore(device, presentCompleteSem, nullptr);
    if (renderCompleteSem)
        vkDestroySemaphore(device, renderCompleteSem, nullptr);

    vkFreeCommandBuffers(device, commandPool, drawCmdBuffers.size(), drawCmdBuffers.data());

    if (renderPass)
        vkDestroyRenderPass(device, renderPass, nullptr);

    for (size_t i = 0; i < frameBuffers.size(); ++i)
        vkDestroyFramebuffer(device, frameBuffers[i], nullptr);

    if (depthStencil.view)
        vkDestroyImageView(device, depthStencil.view, nullptr);
    if (depthStencil.image)
        vkDestroyImage(device, depthStencil.image, nullptr);
    if (depthStencil.mem)
        vkFreeMemory(device, depthStencil.mem, nullptr);

    if (commandPool)
        vkDestroyCommandPool(device, commandPool, nullptr);

    if (device)
        vkDestroyDevice(device, nullptr);
}


void VulkanRenderer::beginFrame()
{
    // TODO this is only for testing
    if (!init)
    {
        pipeline = new VulkanPipeline(device, renderPass);
        auto vertShader = vk::createShader(device, vulkanDevice->getFileSystem()->readBytes("../assets/triangle.vert.spv"));
        auto fragShader = vk::createShader(device, vulkanDevice->getFileSystem()->readBytes("../assets/triangle.frag.spv"));
        pipeline->setVertexShader(vertShader, "main");
        pipeline->setFragmentShader(fragShader, "main");
        pipeline->rebuild();
        init = true;
    }

    swapchain->acquireNextImage(presentCompleteSem, currentBuffer);
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
	submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];

    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

    swapchain->present(queue, currentBuffer, renderCompleteSem);
    SL_CHECK_VK_RESULT(vkQueueWaitIdle(queue));
}


void VulkanRenderer::initCommandBuffers()
{
    static VkClearColorValue defaultClearColor = {{0.025f, 0.025f, 0.025f, 1.0f}};

    auto count = swapchain->getImageCount();

    drawCmdBuffers.resize(count);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(count);

    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, drawCmdBuffers.data()));

    // TODO this is only for testing
    for (size_t i = 0; i < drawCmdBuffers.size(); i++)
    {
        VkClearValue clearValues[2];
		clearValues[0].color = defaultClearColor;
        clearValues[1].depthStencil = {1.0f, 0};

        VkRenderPassBeginInfo renderPassBeginInfo;
        renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = canvasWidth;
		renderPassBeginInfo.renderArea.extent.height = canvasHeight;
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearValues;
        renderPassBeginInfo.framebuffer = frameBuffers[i];

        VkViewport viewport {};
		viewport.width = canvasWidth;
		viewport.height = canvasHeight;
		viewport.minDepth = 0;
		viewport.maxDepth = 1;

        auto buf = drawCmdBuffers[i];

        beginCommandBuffer(buf);

        vkCmdBeginRenderPass(buf, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdSetViewport(drawCmdBuffers[i], 0, 1, &viewport);

        vkCmdEndRenderPass(buf);

        SL_CHECK_VK_RESULT(vkEndCommandBuffer(buf));
    }
}


void VulkanRenderer::beginCommandBuffer(VkCommandBuffer buffer)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    SL_CHECK_VK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo));
}

#endif
