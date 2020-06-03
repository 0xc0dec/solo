/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"
#include "SoloVulkanRenderPass.h"
#include "../sdl/SoloSDLDebugInterface.h"

namespace solo
{
	class VulkanDevice;
	class VulkanRenderer;
	class Device;

	class VulkanDebugInterface final: public SDLDebugInterface
	{
	public:
		explicit VulkanDebugInterface(Device *device);
		~VulkanDebugInterface();

		void renderInto(VkCommandBuffer targetCmdBuffer) const;

		auto renderPass() const -> const VulkanRenderPass& { return renderPass_; }
	
	private:
		VulkanDevice *device_;
		VulkanRenderer *renderer_;
		VulkanRenderPass renderPass_;
		VulkanResource<VkDescriptorPool> descPool_;
		// TODO remove no longer used fields
	};
}

#endif