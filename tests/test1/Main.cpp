#include <iostream>
#include "../../include/Solo.h"

using namespace solo;

#define LOG(msg) std::cout << msg << std::endl;

class TestComponent: public IComponent
{
public:
	static str componentId()
	{
		return "TestComponent";
	};

	virtual str id() override { return componentId(); }


	virtual void update() override
	{

	}
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
		node->addComponent<TestComponent>();
		auto cmp = node->getComponent<TestComponent>(TestComponent::componentId());
		LOG(cmp->id());
	}

	void onEngineStopped() override
	{

	}

	void onBeforeFrame() override
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
