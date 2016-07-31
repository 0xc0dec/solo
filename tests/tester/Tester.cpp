#include "Materials_Test.h"
#include "FrameBuffer_Test.h"
#include "Components_Test.h"
#include "Transform_Test.h"
#include "Device_Test.h"
#include "FileSystem_Test.h"
#include "MeshRenderer_Test.h"
#include "Mesh_Test.h"
#include "ResourcePool_Test.h"

using namespace solo;


class Callback final: public DeviceCallback
{
public:
    Callback(std::function<void(Device*)> run): run(run)
    {
    }

    void onStarted() override final
    {
        run(Device::get());
        Device::get()->stopRunning();
    }

private:
    std::function<void(Device*)> run;
};


void runInStubEngine(std::function<void(Device*)> run, const std::string& logPath)
{
    Device::run(
        DeviceSetup().withMode(DeviceMode::Stub).withLogFilePath(logPath),
        std::make_unique<Callback>(run)
    );
}


void runInRealEngine(std::function<void(Device*)> run, const std::string& logPath)
{
    Device::run(
        DeviceSetup().withMode(DeviceMode::OpenGL).withDimensions(1200, 600).withLogFilePath(logPath),
        std::make_unique<Callback>(run)
    );
}


void runCppUnitTests(Device* device)
{
    Device_Test(device).run();
    Components_Test(device).run();
    Transform_Test(device).run();
    MeshRenderer_Test(device).run();
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
