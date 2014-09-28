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
		_engine->getDevice()->setWindowTitle("Test title");
	}

	void onEngineStopped() override
	{
	}

	void onBeforeFrame(float dt) override
	{
		std::cout << dt << std::endl;
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
	EngineLaunchArgs engineArgs = { 640, 480, 32, 24, false };
	Callback callback(engine);
	engine->setCallback(&callback);
	engine->run(engineArgs);
	return 0;
}