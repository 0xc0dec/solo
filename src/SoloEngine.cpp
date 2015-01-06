#include "SoloEngineCallback.h"
#include "SoloEngine.h"
#include "SoloScene.h"
#include "SoloVideoDriver.h"
#include "SoloDevice.h"
#include "SoloLog.h"
#include "SoloException.h"

using namespace solo;


class EmptyEngineCallback: public EngineCallback
{
public:
	bool onDeviceCloseRequested() override { return true; }
	void onEngineStarted() override {}
	void onEngineStopped() override {}
} emptyCallback;


Engine::Engine()
	: _callback(nullptr),
	_lastUpdateTime(0),
	_timeDelta(0)
{
	_callback = &emptyCallback;
}


Engine::~Engine()
{
}


void Engine::updateTime()
{
	auto time = _device->getLifetime();
	_timeDelta = (time - _lastUpdateTime) / 1000.0f;
	_lastUpdateTime = time;
}


void Engine::doRun(const EngineCreationArgs &args)
{
	_device = Device::create(args);
	_driver = VideoDriver::create();
	_scene = Scene::create();

	_callback->onEngineStarted();

	while (true)
	{
		updateTime();
		_device->beginUpdate();
		_scene->update();
		_scene->render();
		_device->endUpdate();
		if (_device->closeRequested() && _callback->onDeviceCloseRequested())
			break;
	}

	_callback->onEngineStopped();
	_device.reset();
	_scene.reset();
}


void Engine::run(const EngineCreationArgs & args)
{
	try
	{
		doRun(args);
	}
	catch (EngineException &e)
	{
		CRITICAL(e.message);
	}
}


void Engine::setCallback(EngineCallback* callback)
{
	_callback = callback;
	if (!_callback)
		_callback = &emptyCallback;
}


float Engine::getTimeDelta() const
{
	return _timeDelta;
}


ptr<Scene> Engine::getScene() const
{
	return _scene;
}


ptr<Device> Engine::getDevice() const
{
	return _device;
}


ptr<VideoDriver> Engine::getVideoDriver() const
{
	return _driver;
}