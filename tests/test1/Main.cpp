#include "MaterialsTest.h"
#include "ComponentsTest.h"
#include "TransformTest.h"
#include "DeviceTest.h"
#include "SoloManualTest.h"

using namespace solo;

class Callback : public EngineCallback
{
public:
	Callback(Engine* engine):
		_engine(engine)
	{
	}

	virtual void onEngineStarted() override
	{
		_engine->getDevice()->setWindowTitle("Test title");
		MaterialsTest(_engine).run();
		ComponentsTest(_engine).run();
		TransformTest(_engine).run();
		DeviceTest(_engine).run();
		ManualTest(_engine).run();
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
	Engine *_engine;
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
