#include "SoloIEngineCallback.h"
#include "SoloEngine.h"
#include "SoloScene.h"
#include "SoloVideoDriver.h"
#include "SoloDevice.h"
#include "SoloLog.h"
#include "SoloException.h"

using namespace solo;


class EmptyEngineCallback: public IEngineCallback
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
	DEBUG("Starting engine");

	DEBUG("Creating device");
	_device = Device::create(args);

	DEBUG("Creating video driver");
	_driver = VideoDriver::create();

	DEBUG("Creating scene");
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

	DEBUG("Stopping engine");
	_callback->onEngineStopped();
	_device.reset();
	_scene.reset();
	
	DEBUG("Engine stopped");
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


void Engine::setCallback(IEngineCallback* callback)
{
	_callback = callback;
	if (!_callback)
		_callback = &emptyCallback;
}


float Engine::getTimeDelta() const
{
	return _timeDelta;
}


ptr<IScene> Engine::getScene() const
{
	return _scene;
}


ptr<IDevice> Engine::getDevice() const
{
	return _device;
}


ptr<IVideoDriver> Engine::getVideoDriver() const
{
	return _driver;
}