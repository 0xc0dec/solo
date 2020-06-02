/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloDebugInterface.h"
#include "SoloDevice.h"
#include "SoloEnums.h"
#include "vk/SoloVulkanDebugInterface.h"
#include "gl/SoloOpenGLDebugInterface.h"

using namespace solo;

auto DebugInterface::fromDevice(Device *device) -> sptr<DebugInterface>
{
	switch (device->mode())
    {
#ifdef SL_OPENGL_RENDERER
		case DeviceMode::OpenGL:
			return std::make_shared<OpenGLDebugInterface>(device);
#endif
		
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanDebugInterface>(device);
#endif
    	default:
    		panic("Unknown device mode");
    		return nullptr;
    }
}
