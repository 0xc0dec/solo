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
	camera->render(scene, <render_target>, <some_tags>);

	// Render to render target
	camera->render(scene, target);

	// Render to screen
	camera->render(scene, nullptr);

	// Chain of effects
	camera->render(scene, rtt);
	renderer->renderTexture(rtt, rtt2, material);
	renderer->renderTexture(rtt2, rtt3, material);
	camera->render(rtt3, nullptr);

	// Use of a renderer (including custom one)
	renderer->render(scene);
	engine->setRenderer(customRenderer);

	renderer->render(scene, camera, rtt);
	renderer->render(scene, camera, nullptr);

	// In engine:
	scene->update();
	...
	renderer->render(scene); // means "render the entire scene with all cameras"

	// Camera render mode + component methods
	camera->setRenderMode(CameraRenderMode::Forward);
	camera->setRenderMode(CameraRenderMode::Deferred);
	camera->setRenderMode(CameraRenderMode::None);

	// Component methods: invoked only when this component is attached to a node with camera
	void preCameraRender()
	{
		// Can disable standard camera rendering in a way like this
		camera->setRenderMode(CameraRenderMode::None);
	}

	void postCameraRender()
	{
		camera->renderScene(scene, rtt, material);
		graphics->processImage(...)
	}

	TODO cleanup Lua component
*/
