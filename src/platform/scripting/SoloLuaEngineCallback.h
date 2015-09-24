#pragma once

#include <functional>
#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 0
#include <LuaIntf.h>
#include "SoloEngineCallback.h"

namespace solo
{
	class Engine;

	class LuaEngineCallback: public EngineCallback
	{
	public:
		LuaEngineCallback(
			std::function<bool()> onDeviceCloseRequestedFunc,
			std::function<void()> onEngineStartedFunc,
			std::function<void()> onEngineStoppedFunc);

		bool onDeviceCloseRequested() override;
		void onEngineStarted() override;
		void onEngineStopped() override;

		static void setCallback(Engine *engine, LuaIntf::LuaRef& component);

	private:
		std::function<bool()> onDeviceCloseRequestedFunc;
		std::function<void()> onEngineStartedFunc;
		std::function<void()> onEngineStoppedFunc;
	};
}