#include "SoloSDLVulkanDevice.h"

#ifdef SL_VULKAN_RENDERER

#include <SDL_syswm.h>
#ifdef SL_WINDOWS
#   include <windows.h>
#endif

using namespace solo;


SDLVulkanDevice::SDLVulkanDevice(const DeviceSetup& setup):
    SDLDevice(setup)
{
    auto flags = static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI);
    if (setup.fullScreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(setup.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        setup.canvasWidth, setup.canvasHeight, flags);
    if (!window)
        SL_EXCEPTION(InternalException, "Failed to create window");

    VkApplicationInfo appInfo = {};
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

    vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

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

    vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
#endif
}


SDLVulkanDevice::~SDLVulkanDevice()
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}


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
