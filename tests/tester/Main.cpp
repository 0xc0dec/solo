#include "Materials_Test.h"
#include "FrameBuffer_Test.h"
#include "Components_Test.h"
#include "Transform_Test.h"
#include "Device_Test.h"
#include "FileSystem_Test.h"
#include "BitFlags_Test.h"
#include "MeshRenderer_Test.h"
#include "Mesh_Test.h"
#include "ResourcePool_Test.h"

using namespace solo;


void runInStubEngine(std::function<void(Device*)> run, const std::string& logPath)
{
    DeviceCreationArgs args;
    args.mode = DeviceMode::Stub;
    args.logFilePath = logPath;
    auto device = Device::init(args);
    run(device);
    Device::shutdown();
}


void runInRealEngine(std::function<void(Device*)> run, const std::string& logPath)
{
    DeviceCreationArgs args;
    args.mode = DeviceMode::OpenGL;
    args.canvasWidth = 1200;
    args.canvasHeight = 600;
    args.logFilePath = logPath;
    auto device = Device::init(args);
    run(device);
    Device::shutdown();
}


void runCppUnitTests(Device* device)
{
    Device_Test(device).run();
    Components_Test(device).run();
    Transform_Test(device).run();
    MeshRenderer_Test(device).run();
    BitFlags_Test(device).run();
    ResourcePool_Test(device).run();
}


void runCppIntegrationTests(Device* device)
{
    FileSystem_Test(device).run();
    Materials_Test(device).run();
    FrameBuffer_Test(device).run();
    Mesh_Test(device).run();
}


int main()
{
#ifdef SL_DEBUG
    runInStubEngine(runCppUnitTests, "cpp-unit-tests.log");
    runInRealEngine(runCppIntegrationTests, "cpp-integration-tests.log");
#endif
    return 0;
}
