#include "SoloIEngineCallback.h"
#include "SoloEngine.h"
#include "SoloLog.h"
#include "SoloDeviceSDL.h"
#include "SoloException.h"

using namespace solo;


Engine::Engine()
	: _callback(nullptr),
	_lastUpdateTime(0)
{
	_callback = &_emptyCallback;
}


Engine::~Engine()
{
}


void Engine::_run(const EngineLaunchArgs & args)
{
	INFO("Starting engine");

	// SDL is the only available option at this moment
	_device = std::make_shared<DeviceSDL>(args);
	_callback->onEngineStarted();

	while (true)
	{
		auto time = _device->lifetime();
		auto dt = (time - _lastUpdateTime) / 1000.0f;
		_lastUpdateTime = time;
		_callback->onBeforeFrame(dt);
		_device->update();
		if (_device->closeRequested() && _callback->onDeviceCloseRequested())
			break;
	}

	_callback->onEngineStopped();
	_device.reset();
}


void Engine::run(const EngineLaunchArgs & args)
{
	try
	{
		_run(args);
	}
	catch (EngineException &e)
	{
		CRITICAL(e.message);
	}
}


void Engine::setCallback(IEngineCallback* callback)
{
	_callback = callback;
	if (!_callback)
		_callback = &_emptyCallback;
}