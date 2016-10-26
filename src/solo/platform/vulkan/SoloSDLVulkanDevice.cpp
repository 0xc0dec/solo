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

#include "SoloSDLVulkanDevice.h"

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
    SL_ERR_IF(!window, "Failed to create window");

    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "";
	appInfo.pEngineName = "";
	appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
#ifdef SL_WINDOWS
    enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

    VkInstanceCreateInfo instanceCreateInfo {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.pApplicationInfo = &appInfo;

    if (enabledExtensions.size() > 0)
	{
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
	}

    SL_CHECK_VK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));

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

    SL_CHECK_VK_RESULT(vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface));
#endif
}


SDLVulkanDevice::~SDLVulkanDevice()
{
    if (surface)
        vkDestroySurfaceKHR(instance, surface, nullptr);
    if (instance)
        vkDestroyInstance(instance, nullptr);
}


auto SDLVulkanDevice::getCanvasSize() const -> Vector2
{
    return Vector2();
}


void SDLVulkanDevice::saveScreenshot(const std::string& path)
{
}


void SDLVulkanDevice::endUpdate()
{
}
