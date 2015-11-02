#include "Materials_Test.h"
#include "Components_Test.h"
#include "Transform_Test.h"
#include "Device_Test.h"
#include "FileSystem_Test.h"
#include "Resources_Test.h"
#include "ModelRenderer_Test.h"
#include "BitFlags_Test.h"

using namespace solo;


class TestRunner: public EngineCallback
{
public:
	explicit TestRunner(Engine* engine): engine(engine)
	{
	}

	virtual bool onDeviceShutdownRequested() override
	{
		return true;
	}

	virtual void onEngineStarted() override
	{
		init();
		for (auto test : tests)
			test->run();
		engine->getDevice()->requestShutdown();
	}

	virtual void onEngineStopped() override
	{
	}

protected:
	std::list<shared<TestBase>> tests;
	Engine *engine;

	virtual void init() = 0;
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
		tests.push_back(SL_NEW2(MaterialsTest, engine));
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
		tests.push_back(SL_NEW2(Resources_Test, engine));
		tests.push_back(SL_NEW2(FileSystem_Test, engine));
		tests.push_back(SL_NEW2(Device_Test, engine));
		tests.push_back(SL_NEW2(ComponentsAndNodes_Test, engine));
		tests.push_back(SL_NEW2(Transform_Test, engine));
		tests.push_back(SL_NEW2(ModelRenderer_Test, engine));
		tests.push_back(SL_NEW2(BitFlags_Test, engine));
	}
};


EngineCreationArgs openGlArgs
{
	EngineMode::OpenGL,
	800,
	600
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
	auto runner = SL_NEW2(UnitTestRunner, engine.get());
	engine->setCallback(runner);
	engine->run();
}


void runCppIntegrationTests()
{
	auto engine = Engine::create(openGlArgs);
	auto runner = SL_NEW2(IntegrationTestAndDemoRunner, engine.get());
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
	runLuaTests("../tests/scripts/demo/demo.lua");
	return 0;
}
