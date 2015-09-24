#include "SoloLuaEngineCallback.h"
#include "SoloEngine.h"

using namespace solo;


LuaEngineCallback::LuaEngineCallback(
	std::function<bool()> onDeviceCloseRequestedFunc,
	std::function<void()> onEngineStartedFunc,
	std::function<void()> onEngineStoppedFunc):
	onDeviceCloseRequestedFunc(onDeviceCloseRequestedFunc),
	onEngineStartedFunc(onEngineStartedFunc),
	onEngineStoppedFunc(onEngineStoppedFunc)
{
}


bool LuaEngineCallback::onDeviceCloseRequested()
{
	return onDeviceCloseRequestedFunc();
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
	auto onDeviceCloseRequestedFunc = callback.has("onDeviceCloseRequested") ? callback.get<std::function<bool()>>("onDeviceCloseRequested") : [] { return false; };
	auto onEngineStartedFunc = callback.has("onEngineStarted") ? callback.get<std::function<void()>>("onEngineStarted") : [] {};
	auto onEngineStoppedFunc = callback.has("onEngineStoppedFunc") ? callback.get<std::function<void()>>("onEngineStoppedFunc") : [] {};
	auto actualCallback = NEW2(LuaEngineCallback, onDeviceCloseRequestedFunc, onEngineStartedFunc, onEngineStoppedFunc);
	engine->setCallback(actualCallback);
}