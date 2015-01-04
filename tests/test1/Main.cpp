#include "GPUProgramTest.h"
#include "ComponentsTest.h"
#include "TransformTest.h"

using namespace solo;


class Callback : public IEngineCallback
{
public:
	explicit Callback(IEngine* engine)
		: _engine(engine)
	{
	}

	virtual void onEngineStarted() override
	{
		_engine->getDevice()->setWindowTitle("Test title");
		GPUProgramTest().run(_engine);
		ComponentsTest().run(_engine);
		TransformTest().run(_engine);
	}

	virtual void onEngineStopped() override
	{
	}

	virtual void onBeforeFrame() override
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
