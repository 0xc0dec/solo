#include "MaterialsTest.h"
#include "ComponentsAndNodesTest.h"
#include "TransformTest.h"
#include "DeviceTest.h"
#include "FileSystemTest.h"
#include "ResourcesTest.h"
#include "DemoTest.h"

using namespace solo;


class TestRunner: public EngineCallback
{
public:
	explicit TestRunner(Engine* engine): engine(engine)
	{
	}

	virtual bool onDeviceCloseRequested() override
	{
		return true;
	}

	virtual void onEngineStarted() override
	{
		init();
		for (auto test : tests)
			test->run();
		finish();
	}

	virtual void onEngineStopped() override
	{
	}

protected:
	std::list<shared<TestBase>> tests;
	Engine *engine;

	virtual void init() = 0;
	virtual void finish() {}
};


class IntegrationTestAndDemoRunner : public TestRunner
{
public:
	explicit IntegrationTestAndDemoRunner(Engine* engine): TestRunner(engine)
	{
	}

protected:
	virtual void init() override
	{
		tests.push_back(NEW2(MaterialsTest, engine));
		tests.push_back(NEW2(DemoTest, engine));
	}
};


class UnitTestRunner: public TestRunner
{
public:
	explicit UnitTestRunner(Engine* engine): TestRunner(engine)
	{
	}

protected:
	virtual void init() override
	{
		tests.push_back(NEW2(ResourcesTest, engine));
		tests.push_back(NEW2(FileSystemTest, engine));
		tests.push_back(NEW2(DeviceTest, engine));
		tests.push_back(NEW2(ComponentsAndNodesTest, engine));
		tests.push_back(NEW2(TransformTest, engine));
	}

	virtual void finish() override
	{
		engine->getDevice()->requestShutdown();
	}
};


EngineCreationArgs openGlArgs
{
	EngineMode::OpenGL,
	640,
	480
};


EngineCreationArgs stubArgs
{
	EngineMode::Stub,
	1,
	1
};


void runCppUnitTests()
{
	auto engine = Engine::create(stubArgs);
	auto runner = NEW2(UnitTestRunner, engine.get());
	engine->setCallback(runner);
	engine->run();
}


void runCppIntegrationTests()
{
	auto engine = Engine::create(openGlArgs);
	auto runner = NEW2(IntegrationTestAndDemoRunner, engine.get());
	engine->setCallback(runner);
	engine->run();
}


void runLuaTests(const std::string& entryScriptPath)
{
	auto scriptManager = ScriptManager::create();
	scriptManager->executeFile(entryScriptPath);
}


int main()
{
	runCppUnitTests();
	runCppIntegrationTests();
	runLuaTests("../tests/scripts/unit-tests.lua");
	runLuaTests("../tests/scripts/demo.lua");
	return 0;
}
