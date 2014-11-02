#include "SoloIEngineCallback.h"
#include "SoloEngine.h"
#include "SoloLog.h"
#include "SoloException.h"
#include "platform/SoloSDLOpenGLDevice.h"

using namespace solo;


class EmptyEngineCallback: public IEngineCallback
{
public:
	bool onDeviceCloseRequested() override { return true; }
	void onEngineStarted() override {}
	void onEngineStopped() override {}
	void onBeforeFrame() override {}
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
	_device = makePtr<SDLOpenGLDevice>(args);

	INFO("Creating scene");
	_scene = makePtr<Scene>();

	_callback->onEngineStarted();

	while (true)
	{
		auto time = _device->lifetime();
		_deltaTime = (time - _lastUpdateTime) / 1000.0f;
		_lastUpdateTime = time;
		_callback->onBeforeFrame();
		_device->update();
		_scene->update();
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
