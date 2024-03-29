/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloVulkanDevice.h"

#ifdef SL_VULKAN_RENDERER

#include <SDL_syswm.h>
#ifdef SL_WINDOWS
#   include <windows.h>
#endif

using namespace solo;

VulkanDevice::VulkanDevice(const DeviceSetup &setup):
    SDLDevice(setup) {
    initWindow(setup.fullScreen, setup.windowTitle.c_str(), setup.canvasWidth, setup.canvasHeight, 0);

    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "";
    appInfo.pEngineName = "";
    appInfo.apiVersion = VK_API_VERSION_1_0;

    vec<const s8 *> enabledExtensions {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef SL_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
#ifdef SL_DEBUG
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#endif
    };

    vec<const s8 *> enabledLayers {
#ifdef SL_DEBUG
        "VK_LAYER_KHRONOS_validation",
#endif
    };

    VkInstanceCreateInfo instanceInfo {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = nullptr;
    instanceInfo.pApplicationInfo = &appInfo;

    if (!enabledLayers.empty()) {
        instanceInfo.enabledLayerCount = enabledLayers.size();
        instanceInfo.ppEnabledLayerNames = enabledLayers.data();
    }

    if (!enabledExtensions.empty()) {
        instanceInfo.enabledExtensionCount = static_cast<u32>(enabledExtensions.size());
        instanceInfo.ppEnabledExtensionNames = enabledExtensions.data();
    }

    instance_ = VulkanResource<VkInstance> {vkDestroyInstance};
    vk::assertResult(vkCreateInstance(&instanceInfo, nullptr, instance_.cleanRef()));

#ifdef SL_WINDOWS
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window(), &wmInfo);

    const auto hwnd = wmInfo.info.win.window;
    const auto hinstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE));

    VkWin32SurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.flags = 0;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.hinstance = hinstance;
    surfaceInfo.hwnd = hwnd;

    surface_ = VulkanResource<VkSurfaceKHR> {instance_, vkDestroySurfaceKHR};
    vk::assertResult(vkCreateWin32SurfaceKHR(instance_, &surfaceInfo, nullptr, surface_.cleanRef()));
#endif
}

VulkanDevice::~VulkanDevice() {
    shutdownSubsystems();
}

void VulkanDevice::endUpdate() {
}

#endif
