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
	bool onDeviceShutdownRequested() override { return true; }
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
		if (device->shutdownRequested() && callback->onDeviceShutdownRequested())
			break;
	}

	callback->onEngineStopped();
}


void Engine::setCallback(shared<EngineCallback> callback)
{
	this->callback = callback ? callback : SL_NEW<EmptyEngineCallback>();
}

/*
	camera->render(scene, <target_texture>, <some_tags>);

	But how to render an already rendered texture in order to apply shader to it?
	How to render the quad needed for that? Camera is not actually needed for that.
	In Unity Graphics.Blit serves that purpose.

	device->beginUpdate();
	scene->update();
	if (callback->onDeviceRendered())
*/