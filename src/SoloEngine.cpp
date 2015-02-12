#include "SoloEngineCallback.h"
#include "SoloEngine.h"
#include "SoloScene.h"
#include "SoloDevice.h"

using namespace solo;


class EmptyEngineCallback: public EngineCallback
{
public:
	bool onDeviceCloseRequested() override { return true; }
	void onEngineStarted() override {}
	void onEngineStopped() override {}
} emptyCallback;


Engine::Engine()
	: callback(nullptr),
	lastUpdateTime(0),
	timeDelta(0)
{
	callback = &emptyCallback;
}


Engine::~Engine()
{
}


void Engine::updateTime()
{
	auto time = device->getLifetime();
	timeDelta = (time - lastUpdateTime) / 1000.0f;
	lastUpdateTime = time;
}


void Engine::run(const EngineCreationArgs & args)
{
	device = DeviceFactory::create(args);
	scene = Scene::create();

	callback->onEngineStarted();

	while (true)
	{
		updateTime();
		device->beginUpdate();
		scene->update();
		scene->render();
		device->endUpdate();
		if (device->closeRequested() && callback->onDeviceCloseRequested())
			break;
	}

	callback->onEngineStopped();
	device.reset();
	scene.reset();
}


void Engine::setCallback(EngineCallback* callback)
{
	this->callback = callback ? callback : &emptyCallback;
}


float Engine::getTimeDelta() const
{
	return timeDelta;
}


ptr<Scene> Engine::getScene() const
{
	return scene;
}


ptr<Device> Engine::getDevice() const
{
	return device;
}
