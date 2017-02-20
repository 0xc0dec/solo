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

#include "SoloVulkan.h"

#ifdef SL_VULKAN_RENDERER

#include <vector>
#include <array>
#include <tuple>

using namespace solo;

    
auto vk::getPhysicalDevice(VkInstance instance) -> VkPhysicalDevice
{
    uint32_t gpuCount = 0;
    SL_CHECK_VK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));

    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    SL_CHECK_VK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()));

    return physicalDevices[0]; // TODO at least for now
}


auto vk::createDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) -> Resource<VkDevice>
{
    std::vector<float> queuePriorities = {0.0f};
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo deviceCreateInfo {};
    std::vector<VkPhysicalDeviceFeatures> enabledFeatures {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = enabledFeatures.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    Resource<VkDevice> result{vkDestroyDevice};
    SL_CHECK_VK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, result.replace()));

    return result;
}


auto vk::createSemaphore(VkDevice device) -> Resource<VkSemaphore>
{
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    Resource<VkSemaphore> semaphore{device, vkDestroySemaphore};
    SL_CHECK_VK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, semaphore.replace()));

    return semaphore;
}


auto vk::getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface) -> std::tuple<VkFormat, VkColorSpaceKHR>
{
    uint32_t count;
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr));

    std::vector<VkSurfaceFormatKHR> formats(count);
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()));

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return std::make_tuple(VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace);
    return std::make_tuple(formats[0].format, formats[0].colorSpace);
}


auto vk::getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> uint32_t
{
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

    std::vector<VkQueueFamilyProperties> queueProps;
    queueProps.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueProps.data());

    std::vector<VkBool32> presentSupported(count);
    for (uint32_t i = 0; i < count; i++)
        SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupported[i]));

    // TODO support for separate rendering and presenting queues
    for (uint32_t i = 0; i < count; i++)
    {
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupported[i] == VK_TRUE)
            return i;
    }

    SL_ERR("Could not find queue index");
    return 0;
}


auto vk::getDepthFormat(VkPhysicalDevice device) -> VkFormat
{
    // Since all depth formats may be optional, we need to find a suitable depth format to use
    // Start with the highest precision packed format
    std::vector<VkFormat> depthFormats =
    {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM
    };

    for (auto &format : depthFormats)
    {
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(device, format, &formatProps);
        // Format must support depth stencil attachment for optimal tiling
        if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            return format;
    }

    return VK_FORMAT_UNDEFINED;
}


auto vk::createCommandPool(VkDevice device, uint32_t queueIndex) -> Resource<VkCommandPool>
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    Resource<VkCommandPool> commandPool{device, vkDestroyCommandPool};
    SL_CHECK_VK_RESULT(vkCreateCommandPool(device, &poolInfo, nullptr, commandPool.replace()));

    return commandPool;
}


void vk::submitCommandBuffer(VkQueue queue, VkCommandBuffer buffer)
{
    SL_CHECK_VK_RESULT(vkEndCommandBuffer(buffer));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;

    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, nullptr));
    SL_CHECK_VK_RESULT(vkQueueWaitIdle(queue));
}


void vk::createCommandBuffers(VkDevice logicalDevice, VkCommandPool commandPool, uint32_t count, VkCommandBuffer *result)
{
    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = count;
    
    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(logicalDevice, &allocateInfo, result));
}


void vk::destroyCommandBuffers(VkDevice device, VkCommandPool commandPool, VkCommandBuffer *buffers, uint32_t count)
{
    vkFreeCommandBuffers(device, commandPool, count, buffers);
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


auto vk::createRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat) -> Resource<VkRenderPass>
{
    std::array<VkAttachmentDescription, 2> attachments;

    // Color attachment
    attachments[0].format = colorFormat;
    attachments[0].flags = 0;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Depth attachment
    attachments[1].format = depthFormat;
    attachments[1].flags = 0;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorReference{};
    colorReference.attachment = 0;
    colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthReference{};
    depthReference.attachment = 1;
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorReference;
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = &depthReference;
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
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = dependencies.size();
    renderPassInfo.pDependencies = dependencies.data();

    Resource<VkRenderPass> renderPass{device, vkDestroyRenderPass};
    SL_CHECK_VK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, renderPass.replace()));

    return renderPass;
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
    SL_CHECK_VK_RESULT(vkCreateFramebuffer(device, &createInfo, nullptr, frameBuffer.replace()));

    return frameBuffer;
}


auto vk::createShader(VkDevice device, const std::vector<uint8_t>& data) -> Resource<VkShaderModule>
{
    VkShaderModuleCreateInfo shaderModuleInfo {};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.pNext = nullptr;
    shaderModuleInfo.flags = 0;
    shaderModuleInfo.codeSize = data.size();
    shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(data.data());

    Resource<VkShaderModule> module{device, vkDestroyShaderModule};
    SL_CHECK_VK_RESULT(vkCreateShaderModule(device, &shaderModuleInfo, nullptr, module.replace()));

    return module;
}


auto vk::createDepthStencil(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps,
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

    VkMemoryRequirements memReqs;
    Resource<VkImage> image{device, vkDestroyImage};
    SL_CHECK_VK_RESULT(vkCreateImage(device, &imageInfo, nullptr, image.replace()));
    vkGetImageMemoryRequirements(device, image, &memReqs);

    auto memTypeIndex = findMemoryType(physicalDeviceMemProps, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    SL_ERR_IF(memTypeIndex < 0);

    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = memTypeIndex;

    Resource<VkDeviceMemory> mem{device, vkFreeMemory};
    SL_CHECK_VK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, mem.replace()));
    SL_CHECK_VK_RESULT(vkBindImageMemory(device, image, mem, 0));

    viewInfo.image = image;
    Resource<VkImageView> view{device, vkDestroyImageView};
    SL_CHECK_VK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, view.replace()));

    DepthStencil result;
    result.image = std::move(image);
    result.mem = std::move(mem);
    result.view = std::move(view);
    
    return result;
}


auto vk::createRasterizationStateInfo(bool depthClamp, bool discardEnabled, VkCullModeFlags cullMode, VkFrontFace frontFace)
    -> VkPipelineRasterizationStateCreateInfo
{
    VkPipelineRasterizationStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.depthClampEnable = depthClamp;
    info.rasterizerDiscardEnable = discardEnabled;
    info.polygonMode = VK_POLYGON_MODE_FILL;
    info.cullMode = cullMode;
    info.frontFace = frontFace;
    info.depthBiasEnable = false;
    info.depthBiasClamp = 0;
    info.depthBiasConstantFactor = 0;
    info.depthBiasClamp = 0;
    info.depthBiasSlopeFactor = 0;
    info.lineWidth = 0;
    return info;
}


auto vk::createMultisampleStateInfo(VkSampleCountFlagBits rasterizationSampleCount) -> VkPipelineMultisampleStateCreateInfo
{
    VkPipelineMultisampleStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.rasterizationSamples = rasterizationSampleCount;
    info.sampleShadingEnable = false;
    info.minSampleShading = 0;
    info.pSampleMask = nullptr;
    info.alphaToCoverageEnable = false;
    info.alphaToOneEnable = false;
    return info;
}


auto vk::createBlendAttachmentState(bool blendEnabled, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor,
    VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp,
    VkColorComponentFlags colorWriteMask) -> VkPipelineColorBlendAttachmentState
{
    VkPipelineColorBlendAttachmentState state {};
    state.blendEnable = blendEnabled ? VK_TRUE : VK_FALSE;
    state.srcColorBlendFactor = srcColorBlendFactor;
    state.dstColorBlendFactor = dstColorBlendFactor;
    state.colorBlendOp = colorBlendOp;
    state.srcAlphaBlendFactor = srcAlphaBlendFactor;
    state.dstAlphaBlendFactor = dstAlphaBlendFactor;
    state.alphaBlendOp = alphaBlendOp;
    state.colorWriteMask = colorWriteMask;
    return state;
}


auto vk::createColorBlendStateInfo(VkPipelineColorBlendAttachmentState *blendAttachments, bool logicOpEnabled, VkLogicOp logicOp)
    -> VkPipelineColorBlendStateCreateInfo
{
    VkPipelineColorBlendStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.logicOpEnable = logicOpEnabled ? VK_TRUE : VK_FALSE;
    info.logicOp = logicOp;
    info.attachmentCount = 1;
    info.pAttachments = blendAttachments;
    info.blendConstants[0] = 0;
    info.blendConstants[1] = 0;
    info.blendConstants[2] = 0;
    info.blendConstants[3] = 0;
    return info;
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
