#include "MaterialsTest.h"
#include "ComponentsAndNodesTest.h"
#include "TransformTest.h"
#include "DeviceTest.h"
#include "FileSystemTest.h"
#include "ResourcesTest.h"
#include "DemoTest.h"

using namespace solo;


class Callback : public EngineCallback
{
public:
	Callback(Engine* engine): engine(engine)
	{
	}

	virtual void onEngineStarted() override
	{
		tests.push_back(NEW2(MaterialsTest, engine));
		tests.push_back(NEW2(ComponentsAndNodesTest, engine));
		tests.push_back(NEW2(TransformTest, engine));
		tests.push_back(NEW2(DeviceTest, engine));
		tests.push_back(NEW2(FileSystemTest, engine));
		tests.push_back(NEW2(ResourcesTest, engine));
		tests.push_back(NEW2(DemoTest, engine));
		for (auto test : tests)
			test->run();
	}

	virtual void onEngineStopped() override
	{
	}

	virtual bool onDeviceCloseRequested() override
	{
		return true;
	}

private:
	std::list<shared<TestBase>> tests; // to keeps tests in memory
	Engine *engine;
};


void runEngine(EngineMode mode)
{
	auto engine = Engine::create(EngineCreationArgs{ mode, 640, 480 });
	try
	{
		Callback callback(engine.get());
		engine->setCallback(&callback);
		engine->run();
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}


int main()
{
	runEngine(EngineMode::OpenGL);
	runEngine(EngineMode::OpenGL);
	return 0;
}
