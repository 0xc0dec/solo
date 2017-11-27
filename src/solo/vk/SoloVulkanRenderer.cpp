/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanRenderer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanSDLDevice.h"
#include "SoloVulkanMaterial.h"
#include "SoloVulkanMesh.h"
#include "SoloVulkanEffect.h"
#include "SoloVulkanDescriptorSetLayoutBuilder.h"
#include "SoloVulkanDescriptorSetUpdater.h"
#include "SoloVulkanTexture.h"
#include "SoloCamera.h"

using namespace solo;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
    u64 obj, size_t location, s32 code, const s8 *layerPrefix, const s8 *msg, void *userData)
{
    SL_PANIC(msg);
    return VK_FALSE;
}

static auto createDebugCallback(VkInstance instance, PFN_vkDebugReportCallbackEXT callbackFunc) -> VulkanResource<VkDebugReportCallbackEXT>
{
    VkDebugReportCallbackCreateInfoEXT createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = callbackFunc;

    auto create = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
    SL_PANIC_IF(!create, "Failed to load pointer to vkCreateDebugReportCallbackEXT");

    auto destroy = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
    SL_PANIC_IF(!destroy, "Failed to load pointer to vkDestroyDebugReportCallbackEXT");

    VulkanResource<VkDebugReportCallbackEXT> result{instance, destroy};
    SL_VK_CHECK_RESULT(create(instance, &createInfo, nullptr, result.cleanRef()));

    return result;
}

static auto createDevice(VkPhysicalDevice physicalDevice, u32 queueIndex) -> VulkanResource<VkDevice>
{
    vec<float> queuePriorities = {0.0f};
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    vec<const s8*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo deviceCreateInfo {};
    vec<VkPhysicalDeviceFeatures> enabledFeatures {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = enabledFeatures.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VulkanResource<VkDevice> result{vkDestroyDevice};
    SL_VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, result.cleanRef()));

    return result;
}

static auto getPhysicalDevice(VkInstance instance) -> VkPhysicalDevice
{
    u32 gpuCount = 0;
    SL_VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));

    vec<VkPhysicalDevice> devices(gpuCount);
    SL_VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, devices.data()));

    return devices[0]; // Taking first one for simplicity
}

static auto getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface) -> std::tuple<VkFormat, VkColorSpaceKHR>
{
    u32 count;
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr));

    vec<VkSurfaceFormatKHR> formats(count);
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()));

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return {VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace};
    return {formats[0].format, formats[0].colorSpace};
}

static auto getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> u32
{
    u32 count;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

    vec<VkQueueFamilyProperties> queueProps;
    queueProps.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueProps.data());

    vec<VkBool32> presentSupported(count);
    for (u32 i = 0; i < count; i++)
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupported[i]));

    // TODO support for separate rendering and presenting queues
    for (u32 i = 0; i < count; i++)
    {
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupported[i] == VK_TRUE)
            return i;
    }

    SL_PANIC("Could not find queue index");
    return 0;
}

static auto createCommandPool(VkDevice device, u32 queueIndex) -> VulkanResource<VkCommandPool>
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VulkanResource<VkCommandPool> commandPool{device, vkDestroyCommandPool};
    SL_VK_CHECK_RESULT(vkCreateCommandPool(device, &poolInfo, nullptr, commandPool.cleanRef()));

    return commandPool;
}

static auto getDepthFormat(VkPhysicalDevice device) -> VkFormat
{
    // Since all depth formats may be optional, we need to find a suitable depth format to use
    // Start with the highest precision packed format
    vec<VkFormat> depthFormats =
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

VulkanRenderer::VulkanRenderer(Device *engineDevice):
    engineDevice(engineDevice)
{
    const auto vulkanDevice = dynamic_cast<VulkanSDLDevice*>(engineDevice);
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
    swapchain = VulkanSwapchain(this, vulkanDevice, canvasSize.x, canvasSize.y, engineDevice->isVsync());
}

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::beginFrame()
{
    renderCommands.clear();
    renderCommands.reserve(100); // TODO just picked random constant

    pipelines.clear();
}

void VulkanRenderer::endFrame()
{
    crappyName();

    auto presentCompleteSem = swapchain.acquire();
    swapchain.submitAndPresent(queue, 1, &presentCompleteSem);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(queue));
}

void VulkanRenderer::crappyName()
{
    const auto buf = swapchain.getCurrentCmdBuffer();
    const auto frameBuffer = swapchain.getCurrentFrameBuffer();
    auto &renderPass = swapchain.getRenderPass();
    
    vk::beginCommandBuffer(buf, false);

    const auto canvasSize = engineDevice->getCanvasSize();
    renderPass.begin(buf, frameBuffer, canvasSize.x, canvasSize.y);

    const Camera *currentCamera = nullptr;

    for (const auto &cmd : renderCommands)
    {
        switch (cmd.type)
        {
            case RenderCommandType::BeginCamera:
            {
                currentCamera = cmd.camera;

                const auto camViewport = cmd.camera->getViewport();
                VkViewport vp{camViewport.x, camViewport.y, camViewport.z, camViewport.w, 0, 1};
                vkCmdSetViewport(buf, 0, 1, &vp);

                VkRect2D scissor{{0, 0}, {vp.width, vp.height}}; // TODO proper values
                vkCmdSetScissor(buf, 0, 1, &scissor);

                break;
            }

            case RenderCommandType::EndCamera:
                currentCamera = nullptr;
                break;

            case RenderCommandType::DrawMesh:
            {
                break;
            }

            case RenderCommandType::DrawMeshPart:
            {
                const auto material = static_cast<VulkanMaterial*>(cmd.meshPart.material);
                const auto effect = static_cast<VulkanEffect*>(material->getEffect());
                const auto transform = cmd.meshPart.transform;
                const auto mesh = static_cast<VulkanMesh*>(cmd.meshPart.mesh);
                const auto vs = effect->getVertexShader();
                const auto fs = effect->getFragmentShader();
                const auto &uniformBufs = effect->getUniformBuffers();
                const auto &effectSamplers = effect->getSamplers();
                const auto &materialSamplers = material->getSamplers();
                auto &binding = nodeMaterialBindings[material][transform][currentCamera];

                if (!binding.descSet) // new binding
                {
                    auto builder = VulkanDescriptorSetLayoutBuilder(device);

                    for (const auto &info: uniformBufs)
                    {
                        const auto bufferName = info.first;
                        binding.buffers[bufferName] = VulkanBuffer::createUniformHostVisible(this, info.second.size);
                        builder.withBinding(info.second.binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL_GRAPHICS);
                    }

                    for (auto &pair : effectSamplers)
                        builder.withBinding(pair.second.binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);

                    binding.descSetLayout = builder.build();

                    auto poolConfig = VulkanDescriptorPoolConfig();
                    poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniformBufs.size());
                    poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, effectSamplers.size());

                    binding.descPool = VulkanDescriptorPool(device, 1, poolConfig);
                    binding.descSet = binding.descPool.allocateSet(binding.descSetLayout);

                    // TODO Invoke updater outside of the binding initialization because at least
                    // material sampler parameters may change in future

                    // TODO Invoke updater not so often - only when something really changes
                    VulkanDescriptorSetUpdater updater{device};

                    // TODO Not necessary (?), buffers don't change anyway, only their content
                    for (auto &pair : binding.buffers)
                    {
                        const auto info = uniformBufs.at(pair.first);
                        auto &buffer = pair.second;
                        updater.forUniformBuffer(info.binding, binding.descSet, buffer, 0, info.size); // TODO use single large buffer?
                    }

                    for (auto &pair : materialSamplers)
                    {
                        auto &info = pair.second;
                        updater.forTexture(
                            info.binding,
                            binding.descSet,
                            info.texture->getImage().getView(),
                            info.texture->getImage().getSampler(),
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                        );
                    }

                    updater.updateSets();
                }

                auto &bufferItems = material->getBufferItems();
                for (auto &p: bufferItems)
                {
                    auto &buffer = binding.buffers[p.first];
                    for (auto &pp: p.second)
                        pp.second.write(buffer, currentCamera, transform);
                }

                auto pipelineConfig = VulkanPipelineConfig(vs, fs)
                    .withDescriptorSetLayout(binding.descSetLayout)
                    .withFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
                    .withCullMode(material->getCullModeFlags())
                    .withPolygonMode(material->getVkPolygonMode())
                    .withTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

                for (s32 i = 0; i < mesh->getVertexBufferCount(); i++)
                    pipelineConfig.withVertexBufferLayout(i, mesh->getVertexBufferLayout(i));

                pipelines.emplace_back(device, renderPass, pipelineConfig);

                vkCmdBindPipeline(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipelines.rbegin());
                vkCmdBindDescriptorSets(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.rbegin()->getLayout(), 0, 1, &binding.descSet, 0, nullptr);

                VkDeviceSize vertexBufferOffset = 0;
                for (u32 i = 0; i < mesh->getVertexBufferCount(); i++)
                {
                    auto vertexBuffer = mesh->getVertexBuffer(i);
                    vkCmdBindVertexBuffers(buf, i, 1, &vertexBuffer, &vertexBufferOffset);
                }

                const auto indexBuffer = mesh->getPartBuffer(cmd.meshPart.part);
                vkCmdBindIndexBuffer(buf, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
                vkCmdDrawIndexed(buf, mesh->getPartIndexElementCount(cmd.meshPart.part), 1, 0, 0, 0);

                break;
            }

            default:
                break;
        }
    }

    renderPass.end(buf);
    SL_VK_CHECK_RESULT(vkEndCommandBuffer(buf));
}

#endif
