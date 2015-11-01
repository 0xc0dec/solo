#include "SoloEngineCallback.h"
#include "SoloEngine.h"
#include "SoloScene.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloResourceManager.h"

using namespace solo;


class EmptyEngineCallback: public EngineCallback
{
public:
	bool onDeviceCloseRequested() override { return true; }
	void onEngineStarted() override {}
	void onEngineStopped() override {}
};


shared<Engine> Engine::create(const EngineCreationArgs& args)
{
	return SL_NEW2(Engine, args);
}


Engine::Engine(const EngineCreationArgs& args):
	creationArgs{args}
{
	callback = SL_NEW<EmptyEngineCallback>();
	device = DeviceFactory::create(creationArgs);
	fs = FileSystemFactory::create();
	resourceManager = ResourceManagerFactory::create(this);
	scene = SceneFactory::create(this);
}


void Engine::run()
{
	callback->onEngineStarted();

	while (true)
	{
		device->beginUpdate();
		scene->update();
		scene->render();
		device->endUpdate();
		if (device->shutdownRequested() && callback->onDeviceCloseRequested())
			break;
	}

	callback->onEngineStopped();
}


void Engine::setCallback(shared<EngineCallback> callback)
{
	this->callback = callback ? callback : SL_NEW<EmptyEngineCallback>();
}
