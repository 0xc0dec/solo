/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDebugInterface.h"

namespace solo
{
	class VulkanSDLDevice;
	class VulkanRenderer;
	class Device;
	
	class VulkanSDLDebugInterface final: public DebugInterface
	{
	public:
		explicit VulkanSDLDebugInterface(Device *device);
		~VulkanSDLDebugInterface();

		void beginFrame() override;
		void endFrame() override;
		
	private:
		VulkanSDLDevice *device_;
		VulkanRenderer *renderer_;
	};
}

#endif