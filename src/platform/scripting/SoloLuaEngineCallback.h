//#pragma once
//
//#include <functional>
//#include <LuaIntf.h>
//#include "SoloEngineCallback.h"
//
//namespace solo
//{
//	class Engine;
//
//	class LuaEngineCallback: public EngineCallback
//	{
//	public:
//		LuaEngineCallback(
//			std::function<bool()> onDeviceShutdownRequestedFunc,
//			std::function<void()> onEngineStartedFunc,
//			std::function<void()> onEngineStoppedFunc);
//
//		bool onDeviceShutdownRequested() override;
//		void onEngineStarted() override;
//		void onEngineStopped() override;
//
//		static void setCallback(Engine *engine, LuaIntf::LuaRef& component);
//
//	private:
//		std::function<bool()> onDeviceShutdownRequestedFunc;
//		std::function<void()> onEngineStartedFunc;
//		std::function<void()> onEngineStoppedFunc;
//	};
//}