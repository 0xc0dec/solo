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
#include "SoloVulkanDescriptorSetLayoutBuilder.h"
#include "SoloVulkanDescriptorPool.h"
#include "SoloVulkanPipeline.h"
#include "SoloVulkanBuffer.h"
#include "SoloVulkanSwapchain.h"
// TODO remove?
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloFileSystem.h"

using namespace solo;

// TODO remove
Device *engineDevice = nullptr;


VulkanRenderer::VulkanRenderer(Device *engineDevice):
    canvasWidth(engineDevice->getSetup().canvasWidth),
    canvasHeight(engineDevice->getSetup().canvasHeight)
{
    ::engineDevice = engineDevice; // TODO remove

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
    semaphores.presentComplete = vk::createSemaphore(device);
    semaphores.renderComplete = vk::createSemaphore(device);
    commandPool = vk::createCommandPool(device, queueIndex);
    depthStencil = vk::createDepthStencil(device, memProperties, depthFormat, canvasWidth, canvasHeight);
    renderPass = vk::createRenderPass(device, colorFormat, depthFormat);
    
    swapchain = std::make_shared<VulkanSwapchain>(device, physicalDevice, surface, renderPass, depthStencil.view,
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


// TODO remove
bool initialized = false;


void VulkanRenderer::beginFrame()
{
    // TODO remove
//    if (!initialized)
//    {
//        initTest(engineDevice);
//        initialized = true;
//    }
    
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


// TODO Remove after testing
struct Vertex
{
    Vector2 position;
    Vector3 color;
};

VulkanDescriptorPool descriptorPool;
VkDescriptorSetLayout descSetLayout;
VulkanBuffer vertexBuffer;
VulkanBuffer uniformBuffer;
VkPipeline pipeline = nullptr;


void VulkanRenderer::initTest(Device *engineDevice)
{
    VulkanDescriptorSetLayoutBuilder builder(device);
    builder.setBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL_GRAPHICS);
    descSetLayout = builder.build();

    descriptorPool = VulkanDescriptorPool(device, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 1);
    auto descriptorSet = descriptorPool.allocateSet(descSetLayout);

    auto vertexShader = vk::createShader(device, engineDevice->getFileSystem()->readBytes("../assets/triangle.vert.spv"));
    auto fragmentShader = vk::createShader(device, engineDevice->getFileSystem()->readBytes("../assets/triangle.frag.spv"));

    VulkanPipeline pipeline(device, renderPass);
    pipeline.setVertexShader(vertexShader, "main");
    pipeline.setFragmentShader(fragmentShader, "main");

    pipeline.setDescriptorSetLayouts(&descSetLayout, 1);

    pipeline.setVertexSize(sizeof(Vertex));

    pipeline.setVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position));
    pipeline.setVertexAttribute(1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, color));

    pipeline.setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    pipeline.setCullMode(VK_CULL_MODE_BACK_BIT);
    pipeline.setFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
    pipeline.setRasterizationSampleCount(VK_SAMPLE_COUNT_1_BIT);
    pipeline.setColorBlend(VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD);
    pipeline.setAlphaBlend(VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD);
    pipeline.setColorWriteMask(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

    pipeline.rebuild();

    const auto triangleSize = 1.6f;
    std::vector<Vertex> triangle1 =
    {
		{ Vector2(0.5f * triangleSize, sqrtf(3.0f) * 0.25f * triangleSize), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector2(0.0f, -sqrtf(3.0f) * 0.25f * triangleSize), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector2(-0.5f * triangleSize, sqrtf(3.0f) * 0.25f * triangleSize), Vector3(0.0f, 0.0f, 1.0f) }
	};

    std::vector<Vertex> triangle2 =
    {
		{ Vector2(0.3f * triangleSize, sqrtf(3.0f) * 0.25f * triangleSize), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector2(0.0f, -sqrtf(3.0f) * 0.25f * triangleSize), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector2(-0.5f * triangleSize, sqrtf(3.0f) * 0.25f * triangleSize), Vector3(0.0f, 0.0f, 1.0f) }
	};

    auto stagingBuffer = VulkanBuffer(device, sizeof(decltype(triangle1)::value_type) * triangle1.size(),
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        memProperties);
    stagingBuffer.update(triangle2.data());
    stagingBuffer.update(triangle1.data());
    stagingBuffer.update(triangle2.data()); // just smoking

    vertexBuffer = VulkanBuffer(device, sizeof(decltype(triangle1)::value_type) * triangle1.size(),
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        memProperties);
    stagingBuffer.transferTo(vertexBuffer, queue, commandPool);

    auto uniformColor = Vector4(1, 1, 0, 1);
    uniformBuffer = VulkanBuffer(device, sizeof(Vector4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, memProperties);
    uniformBuffer.update(&uniformColor);

    VkDescriptorBufferInfo uboInfo {};
    uboInfo.buffer = uniformBuffer.getHandle();
    uboInfo.offset = 0;
    uboInfo.range = sizeof(Vector3);

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &uboInfo;
    descriptorWrite.pImageInfo = nullptr; // Optional
    descriptorWrite.pTexelBufferView = nullptr; // Optional

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
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

        vk::recordCommandBuffer(cmdBuffers[i], [=](VkCommandBuffer buf)
        {
            vkCmdBeginRenderPass(buf, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdSetViewport(buf, 0, 1, &viewport);
            vkCmdSetScissor(buf, 0, 1, &scissor);

            vkCmdEndRenderPass(buf);
        });
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
