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

#include "Components_Test.h"
#include "Transform_Test.h"
#include "Device_Test.h"
#include "FileSystem_Test.h"
#include "MeshRenderer_Test.h"

using namespace solo;


void runInNullEngine(std::function<void(Device *)> run, const std::string &logPath)
{
    auto device = Device::create(DeviceSetup().withMode(DeviceMode::Null).withLogFilePath(logPath));
    run(device.get());
}


void runInRealEngine(std::function<void(Device *)> run, const std::string &logPath)
{
    auto device = Device::create(DeviceSetup().withMode(DeviceMode::OpenGL).withDimensions(1200, 600).withLogFilePath(logPath));
    run(device.get());
}


void runCppUnitTests(Device *device)
{
    Device_Test(device).run();
    Components_Test(device).run();
    Transform_Test(device).run();
    MeshRenderer_Test(device).run();
}


void runCppIntegrationTests(Device *device)
{
    FileSystem_Test(device).run();
}


void runLuaUnitTests(Device *device)
{
    device->getScriptRuntime()->executeFile("../src/tests/tests.lua");
}


int main()
{
#ifdef SL_DEBUG
    runInNullEngine(runCppUnitTests, "cpp-unit-tests.log");
    runInNullEngine(runLuaUnitTests, "lua-unit-tests.log");
    runInRealEngine(runCppIntegrationTests, "cpp-integration-tests.log");
#endif
    return 0;
}
