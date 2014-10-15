#include <iostream>
#include "Solo.h"

using namespace solo;

class Callback : public IEngineCallback
{
public:
	explicit Callback(IEngine* i_engine)
		: _engine(i_engine)
	{
	}

	void onEngineStarted() override
	{
		_engine->device()->setWindowTitle("Test title");
	}

	void onEngineStopped() override
	{
	}

	void onBeforeFrame(float dt) override
	{
	}

	bool onDeviceCloseRequested() override
	{
		return true;
	}

private:
	IEngine *_engine;
};


int main(int argc, char *argv[])
{
	auto engine = getEngine();
	EngineLaunchArgs engineArgs = { 1366, 768, 32, 24, false };
	Callback callback(engine);
	engine->setCallback(&callback);
	engine->run(engineArgs);
	return 0;
}