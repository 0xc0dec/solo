#include "MaterialsTest.h"
#include "ComponentsTest.h"
#include "TransformTest.h"
#include "DeviceTest.h"
#include "FileSystemTest.h"
#include "ResourcesTest.h"
#include "Demo.h"


using namespace solo;

class Callback : public EngineCallback
{
public:
	Callback(Engine* engine): engine(engine)
	{
	}

	virtual void onEngineStarted() override
	{
		/*
		tests.push_back(NEW2(MaterialsTest, engine));
		tests.push_back(NEW2(ComponentsTest, engine));
		tests.push_back(NEW2(TransformTest, engine));
		tests.push_back(NEW2(DeviceTest, engine));
		tests.push_back(NEW2(FileSystemTest, engine));
		tests.push_back(NEW2(ResourcesTest, engine));
		tests.push_back(NEW2(Demo, engine));
		for (auto test : tests)
			test->run();
		*/
		engine->getScripter()->execFile("../data/main.chai");
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


int main()
{
	auto engine = getEngine();
	EngineCreationArgs engineArgs { 640, 480 };
	Callback callback(engine);
	engine->setCallback(&callback);
	engine->run(engineArgs);
	return 0;
}
