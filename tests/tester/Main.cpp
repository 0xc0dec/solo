#include "Materials_Test.h"
#include "FrameBuffer_Test.h"
#include "Components_Test.h"
#include "Transform_Test.h"
#include "Device_Test.h"
#include "FileSystem_Test.h"
#include "Resources_Test.h"
#include "BitFlags_Test.h"
#include "MeshRenderer_Test.h"
#include "Mesh_Test.h"
#include "ResourcePool_Test.h"

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
    Resources_Test(device.get()).run();
    Device_Test(device.get()).run();
    Components_Test(device.get()).run();
    Transform_Test(device.get()).run();
    MeshRenderer_Test(device.get()).run();
    BitFlags_Test(device.get()).run();
    ResourcePool_Test(device.get()).run();
}


void runCppIntegrationTests()
{
    auto device = Device::create(openGlArgs);
    FileSystem_Test(device.get()).run();
    Materials_Test(device.get()).run();
    FrameBuffer_Test(device.get()).run();
    Mesh_Test(device.get()).run();
}


void runLuaTests(const std::string &entryScriptPath)
{
    auto scriptManager = ScriptManager::create();
    scriptManager->executeFile(entryScriptPath);
}


int main()
{
#ifdef SL_DEBUG
    runCppUnitTests();
    runCppIntegrationTests();
    runLuaTests("../tests/scripts/smoke-tests/tests.lua");
#endif
    runLuaTests("../tests/scripts/demo/demo.lua");

    return 0;
}
