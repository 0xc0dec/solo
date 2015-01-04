#include "SoloIEngineCallback.h"
#include "SoloEngine.h"
#include "SoloLog.h"
#include "SoloException.h"
#include "platform/SoloDeviceSDLGL.h"
#include "platform/SoloVideoDriverOpenGL.h"

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


void Engine::_updateTime()
{
	auto time = _device->getLifetime();
	_timeDelta = (time - _lastUpdateTime) / 1000.0f;
	_lastUpdateTime = time;
}


void Engine::_run(const EngineCreationArgs &args)
{
	INFO("Starting engine");

	// SDL is the only available option right now
	INFO("Creating device");
	_device = NEW<DeviceSDLGL>(args);

	INFO("Creating video driver");
	_driver = NEW<VideoDriverOpenGL>();

	INFO("Creating scene");
	_scene = NEW<Scene>();

	_callback->onEngineStarted();

	while (true)
	{
		_updateTime();
		_device->beginUpdate();
		_scene->update();
		_scene->render();
		_device->endUpdate();
		if (_device->closeRequested() && _callback->onDeviceCloseRequested())
			break;
	}

	INFO("Stopping engine");
	_callback->onEngineStopped();
	_device.reset();
	_scene.reset();
	
	INFO("Engine stopped");
}


void Engine::run(const EngineCreationArgs & args)
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