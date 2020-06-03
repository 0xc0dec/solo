/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"

namespace solo
{
	class Device;
	class Renderer;

	class WindowConfig
	{
	public:
		str title;
		Vector2 position;
		Vector2 pivot;
		float alpha = 0;
		bool autoResize = false;
		bool movable = true;
		bool decoration = true;
	};
	
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

		void renderWindow(const WindowConfig &cfg, const std::function<void()> &renderInside);
		void renderText(const str &text);

	protected:
		explicit DebugInterface(Device *device);
		
		virtual void beginFrame() = 0;

	private:
		Renderer *renderer_ = nullptr;
		
		void endFrame();
	};
}
