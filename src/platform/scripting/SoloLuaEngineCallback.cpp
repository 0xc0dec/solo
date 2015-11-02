#include "SoloLuaEngineCallback.h"
#include "SoloEngine.h"

using namespace solo;


LuaEngineCallback::LuaEngineCallback(
	std::function<bool()> onDeviceShutdownRequestedFunc,
	std::function<void()> onEngineStartedFunc,
	std::function<void()> onEngineStoppedFunc):
	onDeviceShutdownRequestedFunc(onDeviceShutdownRequestedFunc),
	onEngineStartedFunc(onEngineStartedFunc),
	onEngineStoppedFunc(onEngineStoppedFunc)
{
}


bool LuaEngineCallback::onDeviceShutdownRequested()
{
	return onDeviceShutdownRequestedFunc();
}


void LuaEngineCallback::onEngineStarted()
{
	onEngineStartedFunc();
}


void LuaEngineCallback::onEngineStopped()
{
	onEngineStoppedFunc();
}


void LuaEngineCallback::setCallback(Engine* engine, LuaIntf::LuaRef& callback)
{
	auto onDeviceShutdownRequestedFunc = callback.has("onDeviceShutdownRequested") ? callback.get<std::function<bool()>>("onDeviceShutdownRequested") : [] { return false; };
	auto onEngineStartedFunc = callback.has("onEngineStarted") ? callback.get<std::function<void()>>("onEngineStarted") : [] {};
	auto onEngineStoppedFunc = callback.has("onEngineStoppedFunc") ? callback.get<std::function<void()>>("onEngineStoppedFunc") : [] {};
	auto actualCallback = SL_NEW2(LuaEngineCallback, onDeviceShutdownRequestedFunc, onEngineStartedFunc, onEngineStoppedFunc);
	engine->setCallback(actualCallback);
}