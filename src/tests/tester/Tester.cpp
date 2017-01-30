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


void runInEngine(DeviceMode mode, const std::string& logPath, std::function<void(Device*)> run)
{
    DeviceSetup setup;
    setup.mode = mode;
    setup.canvasWidth = 1200;
    setup.canvasHeight = 600;
    setup.logFilePath = logPath;

    auto device = Device::create(setup);

    try
    {
        run(device.get());
    }
    catch (EngineException& e)
    {
        device->getLogger()->logCritical(e.what());
    }
}


void runCppUnitTests(Device* device)
{
    Device_Test(device).run();
    Components_Test(device).run();
    Transform_Test(device).run();
    MeshRenderer_Test(device).run();
}


void runCppIntegrationTests(Device* device)
{
    FileSystem_Test(device).run();
}


void runLuaUnitTests(Device* device)
{
    device->getScriptRuntime()->executeFile("../../src/tests/tests.lua");
}


int main()
{
#ifdef SL_DEBUG
    runInEngine(DeviceMode::Null, "cpp-unit-tests.log", runCppUnitTests);
    runInEngine(DeviceMode::Null, "lua-unit-tests.log", runLuaUnitTests);
    runInEngine(DeviceMode::OpenGL, "cpp-integration-tests.log", runCppIntegrationTests);
#endif
    return 0;
}
