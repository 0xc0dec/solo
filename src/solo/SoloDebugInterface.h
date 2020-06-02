/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

namespace solo
{
	class Device;
	
	class DebugInterface
	{
	public:
		static auto fromDevice(Device *device) -> sptr<DebugInterface>;
		
		DebugInterface(const DebugInterface &other) = delete;
		DebugInterface(const DebugInterface &&other) = delete;
		virtual ~DebugInterface() = default;

		auto operator=(const DebugInterface &other) -> DebugInterface& = delete;
		auto operator=(const DebugInterface &&other) -> DebugInterface& = delete;

	protected:
		DebugInterface() = default;
	};
}
