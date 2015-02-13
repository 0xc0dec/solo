#include "MaterialsTest.h"
#include "ComponentsTest.h"
#include "TransformTest.h"
#include "DeviceTest.h"
#include "ManualTest.h"

using namespace solo;

class Callback : public EngineCallback
{
public:
	Callback(Engine* engine):
		engine(engine)
	{
	}

	virtual void onEngineStarted() override
	{
		engine->getDevice()->setWindowTitle("Test title");
		MaterialsTest(engine).run();
		ComponentsTest(engine).run();
		TransformTest(engine).run();
		DeviceTest(engine).run();
		ManualTest(engine).run();
		LOG("All tests passed");
	}

	virtual void onEngineStopped() override
	{
	}

	virtual bool onDeviceCloseRequested() override
	{
		return true;
	}

private:
	Engine *engine;
};


int main()
{
	auto engine = getEngine();
	EngineCreationArgs engineArgs = { 640, 480, 32, 24, false };
	Callback callback(engine);
	engine->setCallback(&callback);
	engine->run(engineArgs);
	return 0;
}
