/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

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


void runInNullEngine(std::function<void(Device*)> run, const std::string& logPath)
{
    Device::run(
        DeviceSetup().withMode(DeviceMode::Null).withLogFilePath(logPath),
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
    runInNullEngine(runCppUnitTests, "cpp-unit-tests.log");
    runInRealEngine(runCppIntegrationTests, "cpp-integration-tests.log");
#endif
    return 0;
}
