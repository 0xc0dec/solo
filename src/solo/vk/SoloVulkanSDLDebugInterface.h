/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDebugInterface.h"
#include "SoloVulkan.h"
#include "SoloVulkanCmdBuffer.h"
#include "SoloVulkanRenderPass.h"

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

		void render(VkCommandBuffer targetCmdBuffer) const;

		auto renderPass() const -> const VulkanRenderPass& { return renderPass_; }
		
	private:
		VulkanSDLDevice *device_;
		VulkanRenderer *renderer_;
		VulkanResource<VkSemaphore> finishSemaphore_; // TODO rename?
		VulkanCmdBuffer renderCmdBuf_;
		VulkanRenderPass renderPass_;
		VulkanResource<VkDescriptorPool> descPool_;
		// TODO remove no longer used fields
	};
}

#endif