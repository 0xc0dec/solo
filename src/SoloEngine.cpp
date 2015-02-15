#include "SoloEngineCallback.h"
#include "SoloEngine.h"
#include "SoloScene.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"

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
	resourceManager = ResourceManagerFactory::create();
	scene = SceneFactory::create();

	callback->onEngineStarted();

	while (true)
	{
		updateTime();
		device->beginUpdate();
		scene->update();
		scene->render();
		device->endUpdate();
		if (device->shutdownRequested() && callback->onDeviceCloseRequested())
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


Scene* Engine::getScene() const
{
	return scene.get();
}


Device* Engine::getDevice() const
{
	return device.get();
}


ResourceManager* Engine::getResourceManager() const
{
	return resourceManager.get();
}