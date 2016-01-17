#include "Materials_Test.h"
#include "RenderTargets_Test.h"
#include "ComponentsAndNodes_Test.h"
#include "Transform_Test.h"
#include "Device_Test.h"
#include "FileSystem_Test.h"
#include "Resources_Test.h"
#include "BitFlags_Test.h"
#include "MeshRenderer_Test.h"
#include "Mesh_Test.h"

using namespace solo;


DeviceCreationArgs openGlArgs
{
    DeviceMode::OpenGL,
    800,
    600
};


DeviceCreationArgs stubArgs
{
    DeviceMode::Stub,
    1,
    1
};


void runCppUnitTests()
{
    auto device = Device::create(stubArgs);
    device->setStartCallback([&]
    {
        Resources_Test(device.get()).run();
        Device_Test(device.get()).run();
        ComponentsAndNodes_Test(device.get()).run();
        Transform_Test(device.get()).run();
        MeshRenderer_Test(device.get()).run();
        BitFlags_Test(device.get()).run();
        device->requestShutdown();
    });
    device->run();
}


void runCppIntegrationTests()
{
    auto device = Device::create(openGlArgs);
    device->setStartCallback([&]
    {
        FileSystem_Test(device.get()).run();
        Materials_Test(device.get()).run();
        RenderTargets_Test(device.get()).run();
        Mesh_Test(device.get()).run();
        device->requestShutdown();
    });
    device->run();
}


void runLuaTests(const std::string &entryScriptPath)
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
