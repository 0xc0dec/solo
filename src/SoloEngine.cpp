#include "SoloIEngineCallback.h"
#include "SoloEngine.h"
#include "SoloLog.h"
#include "SoloException.h"
#include "platform/SoloDeviceSDLGL.h"

using namespace solo;


class EmptyEngineCallback: public IEngineCallback
{
public:
	bool processDeviceCloseRequestedEvent() override { return true; }
	void processEngineStartedEvent() override {}
	void processEngineStoppedEvent() override {}
	void processBeforeFrameEvent() override {}
} emptyCallback;


Engine::Engine()
	: _callback(nullptr),
	_lastUpdateTime(0),
	_deltaTime(0)
{
	_callback = &emptyCallback;
}


Engine::~Engine()
{
}


void Engine::_run(const EngineCreationArgs &args)
{
	INFO("Starting engine");

	// SDL is the only available option right now
	INFO("Creating device");
	_device = makePtr<DeviceSDLGL>(args);

	INFO("Creating scene");
	_scene = makePtr<Scene>();

	_callback->processEngineStartedEvent();

	while (true)
	{
		auto time = _device->lifetime();
		_deltaTime = (time - _lastUpdateTime) / 1000.0f;
		_lastUpdateTime = time;
		_callback->processBeforeFrameEvent();
		_device->update();
		_scene->update();
		if (_device->closeRequested() && _callback->processDeviceCloseRequestedEvent())
			break;
	}

	INFO("Stopping engine");
	_callback->processEngineStoppedEvent();
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
