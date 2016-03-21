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


void runInStubEngine(std::function<void(shared<Device>)> run, const std::string& logPath)
{
    DeviceCreationArgs args;
    args.mode = DeviceMode::Stub;
    args.logFilePath = logPath;
    auto device = Device::create(args);
    run(device);
}


void runInRealEngine(std::function<void(shared<Device>)> run, const std::string& logPath)
{
    DeviceCreationArgs args;
    args.mode = DeviceMode::OpenGL;
    args.canvasWidth = 1200;
    args.canvasHeight = 600;
    args.logFilePath = logPath;
    auto device = Device::create(args);
    run(device);
}


void runCppUnitTests(shared<Device> device)
{
    Resources_Test(device.get()).run();
    Device_Test(device.get()).run();
    Components_Test(device.get()).run();
    Transform_Test(device.get()).run();
    MeshRenderer_Test(device.get()).run();
    BitFlags_Test(device.get()).run();
    ResourcePool_Test(device.get()).run();
}


void runCppIntegrationTests(shared<Device> device)
{
    FileSystem_Test(device.get()).run();
    Materials_Test(device.get()).run();
    FrameBuffer_Test(device.get()).run();
    Mesh_Test(device.get()).run();
}


void runLuaUnitTests(shared<Device> device)
{
    device->getScriptManager()->executeFile("../tests/scripts/smoke-tests/tests.lua");
}


void runDemos()
{
    std::vector<std::string> demoScripts
    {
        "../tests/scripts/demos/demo1.lua",
        "../tests/scripts/demos/demo2.lua"
    };

    while (true)
    {
        int demoNumber;
        std::cout << "Enter demo number (1.." << demoScripts.size() << "): ";
        std::cin >> demoNumber;
        if (demoNumber < 1 || demoNumber > demoScripts.size())
        {
            std::cout << "Wrong demo number" << std::endl;
            break;
        }
        runInRealEngine([&](shared<Device> device) { device->getScriptManager()->executeFile(demoScripts[demoNumber - 1]); }, "demo.log");
    }
}


int main()
{
#ifdef SL_DEBUG
    runInStubEngine(runCppUnitTests, "cpp-unit-tests.log");
    runInRealEngine(runCppIntegrationTests, "cpp-integration-tests.log");
    runInStubEngine(runLuaUnitTests, "lua-smoke-tests.log");
#endif
    runDemos();
    return 0;
}
