#include "Materials_Test.h"
#include "Components_Test.h"
#include "Transform_Test.h"
#include "Device_Test.h"
#include "FileSystem_Test.h"
#include "Resources_Test.h"
#include "ModelRenderer_Test.h"
#include "BitFlags_Test.h"

using namespace solo;


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
	engine->setStartCallback([&]
	{
		Resources_Test(engine.get()).run();
		FileSystem_Test(engine.get()).run();
		Device_Test(engine.get()).run();
		ComponentsAndNodes_Test(engine.get()).run();
		Transform_Test(engine.get()).run();
		ModelRenderer_Test(engine.get()).run();
		BitFlags_Test(engine.get()).run();
		engine->requestShutdown();
	});
	engine->run();
}


void runCppIntegrationTests()
{
	auto engine = Engine::create(openGlArgs);
	engine->setStartCallback([&]
	{
		Materials_Test(engine.get()).run();
		engine->requestShutdown();
	});
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
