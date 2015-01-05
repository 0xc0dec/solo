#include "EffectsTest.h"
#include "ComponentsTest.h"
#include "TransformTest.h"

using namespace solo;

class Callback : public IEngineCallback
{
public:
	Callback(IEngine* engine)
		: _engine(engine)
	{
	}

	virtual void onEngineStarted() override
	{
		_engine->getDevice()->setWindowTitle("Test title");
		EffectsTest().run(_engine);
		ComponentsTest().run(_engine);
		TransformTest().run(_engine);

		auto node = _engine->getScene()->createNode();
		auto camera = _engine->getScene()->createCamera(node);
		camera->setClearColor(1, 0, 0, 1);

		auto canvasSize = _engine->getDevice()->getCanvasSize();
		std::cout << FORMAT("Canvas width: ", canvasSize.x, ", height: ", canvasSize.y) << std::endl;
	}

	virtual void onEngineStopped() override
	{
	}

	virtual bool onDeviceCloseRequested() override
	{
		return true;
	}

private:
	IEngine *_engine;
};


int main()
{
	auto engine = getEngine();
	EngineCreationArgs engineArgs = { 1366, 768, 32, 24, false };
	Callback callback(engine);
	engine->setCallback(&callback);
	engine->run(engineArgs);
	return 0;
}
