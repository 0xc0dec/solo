#include "SoloVulkan.h"

using namespace solo;


auto VulkanHelper::createShader(VkDevice device, const std::vector<uint8_t>& data) -> VkShaderModule
{
    VkShaderModuleCreateInfo shaderModuleInfo {};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.pNext = nullptr;
    shaderModuleInfo.flags = 0;
    shaderModuleInfo.codeSize = data.size();
    shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(data.data());

    VkShaderModule module = nullptr;
    SL_CHECK_VK_RESULT(vkCreateShaderModule(device, &shaderModuleInfo, nullptr, &module));

    return module;
}


auto VulkanHelper::findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, uint32_t typeBits,
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


auto VulkanHelper::createDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) -> VkDevice
{
    std::vector<float> queuePriorities = { 0.0f };
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = {};
    queueCreateInfos.resize(1);
    queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[0].queueFamilyIndex = queueIndex;
    queueCreateInfos[0].queueCount = 1;
    queueCreateInfos[0].pQueuePriorities = queuePriorities.data();

    std::vector<const char*> deviceExtensions;
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    VkDeviceCreateInfo deviceCreateInfo = {};
    std::vector<VkPhysicalDeviceFeatures> enabledFeatures = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = enabledFeatures.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VkDevice logicalDevice = nullptr;
    SL_CHECK_VK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice));

    return logicalDevice;
}


auto VulkanHelper::getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> uint32_t
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


void VulkanHelper::setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldLayout,
    VkImageLayout newLayout, VkImageSubresourceRange subresourceRange)
{
    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = nullptr;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.oldLayout = oldLayout;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    switch (oldLayout)
    {
        case VK_IMAGE_LAYOUT_UNDEFINED:
                imageMemoryBarrier.srcAccessMask = 0;
                break;
        case VK_IMAGE_LAYOUT_PREINITIALIZED:
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
                break;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                break;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                break;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                break;
        default:
            SL_ERR("Unknown old image layout");
            break;
    }

    switch (newLayout)
    {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = imageMemoryBarrier.srcAccessMask | VK_ACCESS_TRANSFER_READ_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            if (imageMemoryBarrier.srcAccessMask == 0)
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;
        default:
            SL_ERR("Unknown new image layout");
            break;
    }

    VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(cmdbuffer, srcStageFlags, destStageFlags, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
}


void VulkanHelper::setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask,
    VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = aspectMask;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;
    setImageLayout(cmdbuffer, image, oldLayout, newLayout, subresourceRange);
}


auto VulkanHelper::createFrameBuffer(VkDevice device, VkImageView colorAttachment, VkImageView depthAttachment,
    VkRenderPass renderPass, uint32_t width, uint32_t height) -> VkFramebuffer
{
    VkImageView attachments[] = { colorAttachment, depthAttachment };

    VkFramebufferCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.renderPass = renderPass;
    createInfo.attachmentCount = 2;
    createInfo.pAttachments = attachments;
    createInfo.width = width;
    createInfo.height = height;
    createInfo.layers = 1;

    VkFramebuffer frameBuffer = nullptr;
    SL_CHECK_VK_RESULT(vkCreateFramebuffer(device, &createInfo, nullptr, &frameBuffer));

    return frameBuffer;
}


auto VulkanHelper::createRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat) -> VkRenderPass
{
    VkAttachmentDescription attachments[2] = {};

    // Color attachment
    attachments[0].format = colorFormat;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Depth attachment
    attachments[1].format = depthFormat;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorReference = {};
    colorReference.attachment = 0;
    colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthReference = {};
    depthReference.attachment = 1;
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
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

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.attachmentCount = 2;
    renderPassInfo.pAttachments = attachments;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 0;
    renderPassInfo.pDependencies = nullptr;

    VkRenderPass renderPass = nullptr;
    SL_CHECK_VK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass));

    return renderPass;
}


auto VulkanHelper::createPhysicalDevice(VkInstance instance) -> VkPhysicalDevice
{
    uint32_t gpuCount = 0;
    SL_CHECK_VK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));
    
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    SL_CHECK_VK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()));

    return physicalDevices[0]; // TODO at least for now
}


auto VulkanHelper::createCommandPool(VkDevice logicalDevice, uint32_t queueIndex) -> VkCommandPool
{
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkCommandPool commandPool = nullptr;
    SL_CHECK_VK_RESULT(vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool));

    return commandPool;
}


auto VulkanHelper::createCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool) -> VkCommandBuffer
{
    VkCommandBufferAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    VkCommandBuffer result = nullptr;
    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(logicalDevice, &allocateInfo, &result));

    return result;
}


void VulkanHelper::submitCommandBuffer(VkQueue queue, VkCommandBuffer buffer)
{
    SL_CHECK_VK_RESULT(vkEndCommandBuffer(buffer));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;

    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, nullptr));
    SL_CHECK_VK_RESULT(vkQueueWaitIdle(queue));
}


auto VulkanHelper::getDepthFormat(VkPhysicalDevice device) -> VkFormat
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

    for (auto& format : depthFormats)
    {
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(device, format, &formatProps);
        // Format must support depth stencil attachment for optimal tiling
        if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            return format;
    }

    return VK_FORMAT_UNDEFINED;
}


VulkanDescriptorPool::VulkanDescriptorPool(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t maxSetCount):
    device(device)
{
    VkDescriptorPoolSize poolSize {};
    poolSize.type = type;
    poolSize.descriptorCount = descriptorCount;

    VkDescriptorPoolCreateInfo poolInfo {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = maxSetCount;

    SL_CHECK_VK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool));
}


VulkanDescriptorPool::VulkanDescriptorPool(VulkanDescriptorPool&& other):
    device(std::move(other.device)),
    pool(std::move(other.pool))
{
    other.device = nullptr;
    other.pool = nullptr;
}


VulkanDescriptorPool::~VulkanDescriptorPool()
{
    if (pool)
        vkDestroyDescriptorPool(device, pool, nullptr);
}


auto VulkanDescriptorPool::operator=(VulkanDescriptorPool& other) -> VulkanDescriptorPool&
{
    device = std::move(other.device);
    pool = std::move(other.pool);
    other.device = nullptr;
    other.pool = nullptr;
    return *this;
}


auto VulkanDescriptorPool::allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet
{
    VkDescriptorSetAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    VkDescriptorSet set = nullptr;
    SL_CHECK_VK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &set));

    return set;
}


VulkanDescriptorSetLayoutBuilder::VulkanDescriptorSetLayoutBuilder(VkDevice device):
    device(device)
{
}


void VulkanDescriptorSetLayoutBuilder::setBinding(uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount,
    VkShaderStageFlagBits stageFlags)
{
    if (binding >= bindings.size())
        bindings.resize(binding + 1);
    bindings[binding].binding = binding;
    bindings[binding].descriptorType = descriptorType;
    bindings[binding].descriptorCount = descriptorCount;
    bindings[binding].stageFlags = stageFlags;
    bindings[binding].pImmutableSamplers = nullptr;
}

auto VulkanDescriptorSetLayoutBuilder::build() -> VkDescriptorSetLayout
{
    VkDescriptorSetLayoutCreateInfo layoutInfo {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();

    VkDescriptorSetLayout result;
    SL_CHECK_VK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &result));
    return result;
}