#include <iostream>
#include "../../include/Solo.h"

using namespace solo;

#define LOG(msg) std::cout << msg << std::endl;

class TestComponent: public IComponent
{
public:

};

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
		auto node = _engine->scene()->createNode("Test node");
		LOG(node->name());
		node->addComponent<TestComponent>();
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
	EngineCreationArgs engineArgs = { 800, 600, 32, 24, false };
	Callback callback(engine);
	engine->setCallback(&callback);
	engine->run(engineArgs);
	return 0;
}