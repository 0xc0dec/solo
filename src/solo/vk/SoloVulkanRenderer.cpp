/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanRenderer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloSDLVulkanDevice.h"
#include "SoloVulkanMaterial.h"
#include "SoloVulkanMesh.h"

using namespace solo;
using namespace vk;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
    uint64_t obj, size_t location, int32_t code, const char *layerPrefix, const char *msg, void *userData)
{
    return VK_FALSE;
}

static auto getPhysicalDevice(VkInstance instance) -> VkPhysicalDevice
{
    uint32_t gpuCount = 0;
    SL_VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));

    std::vector<VkPhysicalDevice> devices(gpuCount);
    SL_VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, devices.data()));

    return devices[0]; // Taking first one for simplicity
}

static auto getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface) -> std::tuple<VkFormat, VkColorSpaceKHR>
{
    uint32_t count;
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr));

    std::vector<VkSurfaceFormatKHR> formats(count);
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()));

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return {VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace};
    return {formats[0].format, formats[0].colorSpace};
}

static auto getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> uint32_t
{
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

    std::vector<VkQueueFamilyProperties> queueProps;
    queueProps.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueProps.data());

    std::vector<VkBool32> presentSupported(count);
    for (uint32_t i = 0; i < count; i++)
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupported[i]));

    // TODO support for separate rendering and presenting queues
    for (uint32_t i = 0; i < count; i++)
    {
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupported[i] == VK_TRUE)
            return i;
    }

    SL_PANIC("Could not find queue index");
    return 0;
}

static auto createCommandPool(VkDevice device, uint32_t queueIndex) -> vk::Resource<VkCommandPool>
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    vk::Resource<VkCommandPool> commandPool{device, vkDestroyCommandPool};
    SL_VK_CHECK_RESULT(vkCreateCommandPool(device, &poolInfo, nullptr, commandPool.cleanRef()));

    return commandPool;
}

vk::Renderer::Renderer(Device *engineDevice):
    engineDevice(engineDevice)
{
    auto vulkanDevice = dynamic_cast<SDLDevice*>(engineDevice);
    auto instance = vulkanDevice->getInstance();
    auto surface = vulkanDevice->getSurface();
    auto canvasWidth = engineDevice->getSetup().canvasWidth;
    auto canvasHeight = engineDevice->getSetup().canvasHeight;

    debugCallback = createDebugCallback(instance, debugCallbackFunc);
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &physicalFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalMemoryFeatures);

    auto surfaceFormats = getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);
    depthFormat = ::getDepthFormat(physicalDevice);

    auto queueIndex = getQueueIndex(physicalDevice, surface);
    device = createDevice(physicalDevice, queueIndex);
    vkGetDeviceQueue(device, queueIndex, 0, &queue);

    commandPool = createCommandPool(device, queueIndex);
}

vk::Renderer::~Renderer()
{
}

void vk::Renderer::beginFrame()
{
    //currentSwapchainStep = swapchain.getNextStep(semaphores.presentComplete);
    //prevRenderCommands = std::move(renderCommands);
    //renderCommands = std::vector<RenderCommand>(100); // TODO just picked random constant
}

void vk::Renderer::endFrame()
{
}

void vk::Renderer::applyRenderCommands(VkCommandBuffer buf)
{
    for (const auto &cmd: renderCommands)
    {
        switch (cmd.type)
        {
            case RenderCommandType::BeginCamera:
                break;

            case RenderCommandType::EndCamera:
                break;

            case RenderCommandType::ApplyMaterial:
                break;

            case RenderCommandType::DrawMesh:
            case RenderCommandType::DrawMeshPart:
                break;

            default:
                break;
        }
    }
}

void vk::Renderer::recordCmdBuffers()
{
    
}

#endif
