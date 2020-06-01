/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloDebugInterface.h"
#include "SoloDevice.h"
#include "SoloEnums.h"
#include "vk/SoloVulkanSDLDebugInterface.h"

using namespace solo;

auto DebugInterface::fromDevice(Device *device) -> sptr<DebugInterface>
{
	switch (device->mode())
    {
		// TODO OpenGL
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanSDLDebugInterface>(device);
#endif
    	default:
    		panic("Unknown device mode");
    		return nullptr;
    }
}
