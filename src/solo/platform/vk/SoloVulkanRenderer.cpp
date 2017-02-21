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
#include "SoloVulkanDescriptorSetLayoutBuilder.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloSDLVulkanDevice.h"
#include "SoloVulkanSwapchain.h"
#include "SoloVulkan.h"
#include "SoloVector3.h"

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

// TODO lol remove
#include <fstream>

auto readAllBytes(const char *path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    SL_PANIC_IF(!file.is_open(), SL_FMT("Failed to open file '", path, "'"));
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    auto result = std::vector<uint8_t>(size);
    file.read(reinterpret_cast<char *>(&result[0]), size);
    file.close();
    return result;
}


void vk::Renderer::updateCmdBuffers()
{
    // TODO this is a quick smoke testing
    struct Vertex
    {
        Vector2 position;
        Vector3 color;
    };

    test.descSetLayout = DescriptorSetLayoutBuilder(device)
        .withBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL_GRAPHICS)
        .build();

    auto vertexShader = vk::createShader(device, readAllBytes("../../assets/triangle.vert.spv"));
    auto fragmentShader = vk::createShader(device, readAllBytes("../../assets/triangle.frag.spv"));

    VkDescriptorSetLayout descSetLayout = test.descSetLayout;
    test.pipeline = PipelineBuilder(device, renderPass)
        .withVertexShader(std::move(vertexShader), "main")
        .withFragmentShader(std::move(fragmentShader), "main")
        .withDescriptorSetLayouts(&descSetLayout, 1)
        .withVertexSize(sizeof(Vertex))
        .withVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position))
        .withVertexAttribute(1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, color))
        .withTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
        .withCullMode(VK_CULL_MODE_BACK_BIT)
        .withFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
        .withRasterizationSampleCount(VK_SAMPLE_COUNT_1_BIT)
        .withColorBlend(VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD)
        .withAlphaBlend(VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD)
        .build();

    const auto triangleSize = 1.6f;
    std::vector<Vertex> tri1 =
    {
		{ Vector2(0.5f * triangleSize, sqrtf(3.0f) * 0.25f * triangleSize), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector2(0.0f, -sqrtf(3.0f) * 0.25f * triangleSize), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector2(-0.5f * triangleSize, sqrtf(3.0f) * 0.25f * triangleSize), Vector3(0.0f, 0.0f, 1.0f) }
	};

    std::vector<Vertex> tri2 =
    {
		{ Vector2(0.3f * triangleSize, sqrtf(3.0f) * 0.25f * triangleSize), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector2(0.0f, -sqrtf(3.0f) * 0.25f * triangleSize), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector2(-0.5f * triangleSize, sqrtf(3.0f) * 0.25f * triangleSize), Vector3(0.0f, 0.0f, 1.0f) }
	};

    auto stagingBuffer = Buffer(device, sizeof(decltype(tri1)::value_type) * tri1.size(),
        Buffer::Host | Buffer::TransferSrc, physicalDevice.memProperties);
    stagingBuffer.update(tri2.data());
    stagingBuffer.update(tri1.data());
    stagingBuffer.update(tri2.data()); // just smoking

    test.vertexBuffer = Buffer(device, sizeof(decltype(tri1)::value_type) * tri1.size(),
        Buffer::Device | Buffer::Vertex | Buffer::TransferDst, physicalDevice.memProperties);
    stagingBuffer.transferTo(test.vertexBuffer, queue, commandPool);

    test.descriptorPool = DescriptorPool(device, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 1);
    auto descriptorSet = test.descriptorPool.allocateSet(test.descSetLayout);

    auto uniformColor = Vector4(0, 0.2f, 0.8f, 1);
    test.uniformBuffer = Buffer(device, sizeof(Vector4), Buffer::Uniform | Buffer::Host, physicalDevice.memProperties);
    test.uniformBuffer.update(&uniformColor);

    VkDescriptorBufferInfo uboInfo{};
    uboInfo.buffer = test.uniformBuffer.getHandle();
    uboInfo.offset = 0;
    uboInfo.range = sizeof(Vector3);

    VkWriteDescriptorSet descriptorWrite{};
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

    renderPass.setViewport(0, 0, canvasWidth, canvasHeight);
    renderPass.setScissor(0, 0, canvasWidth, canvasHeight);

    for (size_t i = 0; i < cmdBuffers.size(); i++)
    {
        auto buf = cmdBuffers[i];

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        SL_VK_CHECK_RESULT(vkBeginCommandBuffer(buf, &beginInfo));

        renderPass.begin(buf, swapchain.getFramebuffer(i), canvasWidth, canvasHeight);

        vkCmdBindPipeline(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, test.pipeline);

        VkDeviceSize offset = 0;
        auto vertexBuffer = test.vertexBuffer.getHandle();
	    vkCmdBindVertexBuffers(buf, 0, 1, &vertexBuffer, &offset);

        vkCmdBindDescriptorSets(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, test.pipeline.getLayout(), 0, 1, &descriptorSet, 0, nullptr);

        vkCmdDraw(buf, static_cast<uint32_t>(tri1.size()), 1, 0, 0);

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
