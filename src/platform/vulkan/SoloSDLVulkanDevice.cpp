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


SDLVulkanDevice::SDLVulkanDevice(const DeviceCreationArgs& args):
    SDLDevice(args)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
        SL_EXCEPTION(InternalException, "Failed to initialize SDL");

    auto flags = static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI);
    if (creationArgs.fullScreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(creationArgs.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        creationArgs.canvasWidth, creationArgs.canvasHeight, flags);
    if (!window)
        SL_EXCEPTION(InternalException, "Failed to create window");

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);

#ifdef SL_WINDOWS
    auto hwnd = wmInfo.info.win.window;
    auto hinstance = GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
#endif

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
	
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	SL_CHECK_VK_CALL(vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()), "Failed to enumerate devices");
}


#else
#   error Vulkan renderer is not supported on this platform
#endif
