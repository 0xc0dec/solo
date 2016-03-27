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


void runInStubEngine(std::function<void(sptr<Device>)> run, const std::string& logPath)
{
    DeviceCreationArgs args;
    args.mode = DeviceMode::Stub;
    args.logFilePath = logPath;
    auto device = Device::create(args);
    run(device);
}


void runInRealEngine(std::function<void(sptr<Device>)> run, const std::string& logPath)
{
    DeviceCreationArgs args;
    args.mode = DeviceMode::OpenGL;
    args.canvasWidth = 1200;
    args.canvasHeight = 600;
    args.logFilePath = logPath;
    auto device = Device::create(args);
    run(device);
}


void runCppUnitTests(sptr<Device> device)
{
    Resources_Test(device.get()).run();
    Device_Test(device.get()).run();
    Components_Test(device.get()).run();
    Transform_Test(device.get()).run();
    MeshRenderer_Test(device.get()).run();
    BitFlags_Test(device.get()).run();
    ResourcePool_Test(device.get()).run();
}


void runCppIntegrationTests(sptr<Device> device)
{
    FileSystem_Test(device.get()).run();
    Materials_Test(device.get()).run();
    FrameBuffer_Test(device.get()).run();
    Mesh_Test(device.get()).run();
}


int main()
{
    runInStubEngine(runCppUnitTests, "cpp-unit-tests.log");
    runInRealEngine(runCppIntegrationTests, "cpp-integration-tests.log");
    return 0;
}
