#include "SoloVulkanRenderer.h"
#include "SoloSDLVulkanDevice.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


std::tuple<VkFormat, VkColorSpaceKHR> getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t count;
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr));

    std::vector<VkSurfaceFormatKHR> formats(count);
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()));

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return std::make_tuple(VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace);
    return std::make_tuple(formats[0].format, formats[0].colorSpace);
}


VkSemaphore createSemaphore(VkDevice device)
{
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    VkSemaphore semaphore = nullptr;
    SL_CHECK_VK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphore));

    return semaphore;
}


VulkanRenderer::VulkanRenderer(Device* engineDevice)
{
    auto vulkanDevice = dynamic_cast<SDLVulkanDevice*>(engineDevice);
    auto instance = vulkanDevice->getVkInstance();
    auto surface = vulkanDevice->getVkSurface();

    physicalDevice = vk::getPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &features);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    auto surfaceFormats = getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = vk::getQueueIndex(physicalDevice, surface);
    device = vk::createDevice(physicalDevice, queueIndex);

    vkGetDeviceQueue(device, queueIndex, 0, &queue);

    depthFormat = vk::getDepthFormat(physicalDevice);

    presentCompleteSem = createSemaphore(device);
    renderCompleteSem = createSemaphore(device);

    commandPool = vk::createCommandPool(device, queueIndex);

    /*initSwapchain(surface, engineDevice->getSetup().vsync);
    initCommandBuffers();

    setupCmdBuffer = VulkanHelper::createCommandBuffer(device, commandPool);
    beginCommandBuffer(setupCmdBuffer);

    depthStencil = createDepthStencil(device, physicalDeviceMemProps, setupCmdBuffer, depthFormat, canvasWidth, canvasHeight);
    renderPass = VulkanHelper::createRenderPass(device, colorFormat, depthFormat);
    initFrameBuffers();

    VulkanHelper::submitCommandBuffer(queue, setupCmdBuffer);
    ::destroyCommandBuffers(device, commandPool, &setupCmdBuffer, 1);*/
}


VulkanRenderer::~VulkanRenderer()
{
    if (commandPool)
        vkDestroyCommandPool(device, commandPool, nullptr);

    if (presentCompleteSem)
        vkDestroySemaphore(device, presentCompleteSem, nullptr);
    if (renderCompleteSem)
        vkDestroySemaphore(device, renderCompleteSem, nullptr);

    if (device)
        vkDestroyDevice(device, nullptr);
}


void VulkanRenderer::beginFrame()
{
}


void VulkanRenderer::endFrame()
{
}


#else
#   error Vulkan renderer is not supported on this platform
#endif
