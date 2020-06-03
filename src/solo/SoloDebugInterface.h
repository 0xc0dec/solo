/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

namespace solo
{
	class Device;
	class Renderer;
	
	class DebugInterface
	{
	public:
		static auto fromDevice(Device *device) -> sptr<DebugInterface>;
		
		DebugInterface(const DebugInterface &other) = delete;
		DebugInterface(const DebugInterface &&other) = delete;
		virtual ~DebugInterface() = default;

		auto operator=(const DebugInterface &other) -> DebugInterface& = delete;
		auto operator=(const DebugInterface &&other) -> DebugInterface& = delete;

		void renderFrame(const std::function<void()> &render);

		void magic();

	protected:
		explicit DebugInterface(Device *device);
		
		virtual void beginFrame() = 0;

	private:
		Renderer *renderer_ = nullptr;
		
		void endFrame();
	};
}
