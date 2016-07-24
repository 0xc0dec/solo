#include "SoloSDLVulkanDevice.h"

#include <SDL_syswm.h>
#ifdef SL_WINDOWS
#   include <windows.h>
#endif

#ifdef SL_VULKAN_RENDERER

#define SL_CHECK_VK_CALL(call, message) \
    do { \
        if (call != VK_SUCCESS) \
            SL_EXCEPTION(InternalException, message); \
    } while (0)

using namespace solo;


uint32_t getQueueIndex(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
    SL_EXCEPTION_IF(count <= 0, InternalException, "Failed to get count of graphics queues");

    std::vector<VkQueueFamilyProperties> queueProps;
	queueProps.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queueProps.data());

    std::vector<VkBool32> presentSupported(count);
    for (auto i = 0; i < count; i++)
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupported[i]);

    // TODO support for separate rendering and presenting queues
    for (auto i = 0; i < count; i++)
	{
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupported[i] == VK_TRUE)
            return i;
	}

    SL_EXCEPTION(InternalException, "Failed to detect queue index");
}


VkFormat getSupportedDepthFormat(VkPhysicalDevice physicalDevice)
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
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
		// Format must support depth stencil attachment for optimal tiling
		if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            return format;
	}

    SL_EXCEPTION(InternalException, "Could not detect suitable depth format");
}


std::tuple<VkFormat, VkColorSpaceKHR> getSurfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    uint32_t count;
    SL_CHECK_VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr), "Failed to get surface format count");

    std::vector<VkSurfaceFormatKHR> formats(count);
    SL_CHECK_VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, formats.data()), "Failed to get surface formats");

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return std::make_tuple(VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace);
    return std::make_tuple(formats[0].format, formats[0].colorSpace);
}


SDLVulkanDevice::SDLVulkanDevice(const DeviceCreationArgs& args):
    SDLDevice(args)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
        SL_EXCEPTION(InternalException, "Failed to initialize SDL");

    auto flags = static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI);
    if (args.fullScreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(args.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        args.canvasWidth, args.canvasHeight, flags);
    if (!window)
        SL_EXCEPTION(InternalException, "Failed to create window");

    // Vulkan init

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "";
	appInfo.pEngineName = "";
	appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
#ifdef SL_WINDOWS
    enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

    VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.pApplicationInfo = &appInfo;

    if (enabledExtensions.size() > 0)
	{
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
	}

    SL_CHECK_VK_CALL(vkCreateInstance(&instanceCreateInfo, nullptr, &instance), "Failed to initialize Vulkan");

	uint32_t gpuCount = 0;
    SL_CHECK_VK_CALL(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr), "Failed to get GPU count");

    SL_EXCEPTION_IF(gpuCount == 0, InternalException, "No GPU found");
	
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	SL_CHECK_VK_CALL(vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()), "Failed to enumerate devices");

    physicalDevice = physicalDevices[0]; // TODO at least for now
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

    // Surface
#ifdef SL_WINDOWS
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);

    auto hwnd = wmInfo.info.win.window;
    auto hinstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE));

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = hinstance;
	surfaceCreateInfo.hwnd = hwnd;

    SL_CHECK_VK_CALL(vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface), "Failed to create rendering surface");
#endif

    auto surfaceFormats = getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    // Queue
    auto queueIndex = getQueueIndex(physicalDevice, surface);
    vkGetDeviceQueue(device, queueIndex, 0, &graphicsQueue);

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
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = enabledFeatures.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    SL_CHECK_VK_CALL(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device), "Failed to create logical device");

    depthFormat = getSupportedDepthFormat(physicalDevice);

    // TODO swap chain initialization here

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;

    SL_CHECK_VK_CALL(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &presentCompleteSem), "Failed to create present semaphore");
    SL_CHECK_VK_CALL(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderCompleteSem), "Failed to create render semaphore");

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &presentCompleteSem;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderCompleteSem;

    // TODO uncomment if normal prototypes from vk header don't work
//    auto pvkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(
//        vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR"));
//    auto pvkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(
//        vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
//    auto pvkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(
//        vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
//    auto pvkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(
//        vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR"));
//    auto pvkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(vkGetDeviceProcAddr(device, "vkCreateSwapchainKHR"));
//    auto pvkDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(vkGetDeviceProcAddr(device, "vkDestroySwapchainKHR"));
//    auto pvkGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(vkGetDeviceProcAddr(device, "vkGetSwapchainImagesKHR"));
//    auto pvkAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(vkGetDeviceProcAddr(device, "vkAcquireNextImageKHR"));
//    auto pvkQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(vkGetDeviceProcAddr(device, "vkQueuePresentKHR"));


void SDLVulkanDevice::saveScreenshot(const std::string& path)
{
    // TODO
}


auto SDLVulkanDevice::getCanvasSize() const -> Vector2
{
    // TODO
    return Vector2();
}


void SDLVulkanDevice::endUpdate()
{
    // TODO
}


#else
#   error Vulkan renderer is not supported on this platform
#endif
