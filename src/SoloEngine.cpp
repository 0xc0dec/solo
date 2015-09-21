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
} emptyCallback;


shared<Engine> Engine::create(const EngineCreationArgs& args)
{
	return NEW2(Engine, args);
}


Engine::Engine(const EngineCreationArgs& args):
	creationArgs{args},
	callback{&emptyCallback}
{
}


Engine::~Engine()
{
}


void Engine::run()
{
	device = Device::create(creationArgs);
	fs = FileSystemFactory::create();
	resourceManager = ResourceManagerFactory::create(this);
	scene = SceneFactory::create(this);

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

	scene.reset();
	resourceManager.reset();
	fs.reset();
	device.reset();
}


void Engine::setCallback(EngineCallback* callback)
{
	this->callback = callback ? callback : &emptyCallback;
}


EngineMode Engine::getMode() const
{
	return creationArgs.mode;
}


Scene *Engine::getScene() const
{
	return scene.get();
}


Device *Engine::getDevice() const
{
	return device.get();
}


FileSystem *Engine::getFileSystem() const
{
	return fs.get();
}


ResourceManager *Engine::getResourceManager() const
{
	return resourceManager.get();
}
