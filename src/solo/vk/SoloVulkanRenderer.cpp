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
#include "SoloVulkanEffect.h"
#include "SoloCamera.h"

using namespace solo;
using namespace vk;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
    uint64_t obj, size_t location, int32_t code, const char *layerPrefix, const char *msg, void *userData)
{
    SL_PANIC(msg);
    return VK_FALSE;
}

static auto createDebugCallback(VkInstance instance, PFN_vkDebugReportCallbackEXT callbackFunc) -> Resource<VkDebugReportCallbackEXT>
{
    VkDebugReportCallbackCreateInfoEXT createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = callbackFunc;

    auto create = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
    SL_PANIC_IF(!create, "Failed to load pointer to vkCreateDebugReportCallbackEXT");

    auto destroy = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
    SL_PANIC_IF(!destroy, "Failed to load pointer to vkDestroyDebugReportCallbackEXT");

    Resource<VkDebugReportCallbackEXT> result{instance, destroy};
    SL_VK_CHECK_RESULT(create(instance, &createInfo, nullptr, result.cleanRef()));

    return result;
}

static auto createDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) -> Resource<VkDevice>
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
    SL_VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, result.cleanRef()));

    return result;
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

static auto getDepthFormat(VkPhysicalDevice device) -> VkFormat
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

vk::Renderer::Renderer(Device *engineDevice):
    engineDevice(engineDevice)
{
    const auto vulkanDevice = dynamic_cast<SDLDevice*>(engineDevice);
    const auto instance = vulkanDevice->getInstance();
    const auto surface = vulkanDevice->getSurface();
    const auto canvasSize = engineDevice->getCanvasSize();

    debugCallback = createDebugCallback(instance, debugCallbackFunc);
    physicalDevice = ::getPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &physicalFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalMemoryFeatures);

    auto surfaceFormats = getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);
    depthFormat = ::getDepthFormat(physicalDevice);

    const auto queueIndex = getQueueIndex(physicalDevice, surface);
    device = createDevice(physicalDevice, queueIndex);
    vkGetDeviceQueue(device, queueIndex, 0, &queue);

    commandPool = createCommandPool(device, queueIndex);
    swapchain = Swapchain(this, vulkanDevice, canvasSize.x, canvasSize.y, engineDevice->isVsync());
}

vk::Renderer::~Renderer()
{
}

void vk::Renderer::beginFrame()
{
    renderCommands.clear();
    renderCommands.reserve(100); // TODO just picked random constant

    pipelines.clear();
    pipelines.reserve(100);
}

void vk::Renderer::endFrame()
{
    // TODO Properly set clear values based on camera

    swapchain.recordCommandBuffers([&](VkFramebuffer fb, VkCommandBuffer buf)
    {
        auto canvasSize = engineDevice->getCanvasSize();
        swapchain.getRenderPass().begin(buf, fb, canvasSize.x, canvasSize.y);

        const Camera *currentCamera = nullptr;
        vk::Material *currentMaterial = nullptr;
        const vk::Effect *currentEffect = nullptr;

        for (const auto &cmd: renderCommands)
        {
            switch (cmd.type)
            {
                case RenderCommandType::BeginCamera:
                {
                    currentCamera = cmd.camera;

                    auto camViewport = cmd.camera->getViewport();
                    VkViewport vp{camViewport.x, camViewport.y, camViewport.z, camViewport.w, 0, 1};
                    vkCmdSetViewport(buf, 0, 1, &vp);

                    VkRect2D scissor{{0, 0}, {vp.width, vp.height}}; // TODO proper values
                    vkCmdSetScissor(buf, 0, 1, &scissor);
                    break;
                }

                case RenderCommandType::EndCamera:
                    currentCamera = nullptr;
                    break;

                case RenderCommandType::ApplyMaterial:
                {
                    currentMaterial = static_cast<vk::Material*>(cmd.material);
                    currentEffect = static_cast<vk::Effect*>(currentMaterial->getEffect());
                    break;
                }

                case RenderCommandType::DrawMesh:
                {
                    break;
                }

                case RenderCommandType::DrawMeshPart:
                {
                    if (!currentMaterial)
                        continue;

                    currentMaterial->applyParameters(this, currentCamera, cmd.meshPart.transform);

                    const auto mesh = static_cast<vk::Mesh*>(cmd.meshPart.mesh);
                    auto &renderPass = swapchain.getRenderPass();
                    auto vs = currentEffect->getVertexShader();
                    auto fs = currentEffect->getFragmentShader();

                    auto pipelineConfig = vk::PipelineConfig(vs, fs)
                        .withDescriptorSetLayout(currentMaterial->getDescSetLayout())
                        .withFrontFace(VK_FRONT_FACE_CLOCKWISE)
                        .withCullMode(currentMaterial->getCullModeFlags())
                        .withPolygonMode(currentMaterial->getVkPolygonMode())
                        .withTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

                    for (int32_t binding = 0; binding < mesh->getVertexBufferCount(); binding++)
                        pipelineConfig.withVertexBufferLayout(binding, mesh->getVertexBufferLayout(binding));

                    pipelines.emplace_back(device, renderPass, pipelineConfig);

                    VkDescriptorSet descSet = currentMaterial->getDescSet();
                    vkCmdBindPipeline(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipelines.rbegin());
                    vkCmdBindDescriptorSets(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.rbegin()->getLayout(), 0, 1, &descSet, 0, nullptr);

                    VkDeviceSize vertexBufferOffset = 0;                    
                    for (uint32_t i = 0; i < mesh->getVertexBufferCount(); i++)
                    {
                        auto vertexBuffer = mesh->getVertexBuffer(i);
                        vkCmdBindVertexBuffers(buf, i, 1, &vertexBuffer, &vertexBufferOffset);
                    }
                    
                    auto indexBuffer = mesh->getPartBuffer(cmd.meshPart.part);
                    vkCmdBindIndexBuffer(buf, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
                    vkCmdDrawIndexed(buf, mesh->getPartIndexElementCount(cmd.meshPart.part), 1, 0, 0, 0);

                    break;
                }

                default:
                    break;
            }
        }

        swapchain.getRenderPass().end(buf);
    });

    auto presendCompleteSem = swapchain.acquireNext();
    swapchain.presentNext(queue, 1, &presendCompleteSem);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(queue));
}

#endif
