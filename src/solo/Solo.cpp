/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloDevice.h"
#include "SoloDeviceSetup.h"
#include "SoloScriptRuntime.h"

int main(int argc, char **argv)
{
    using namespace solo;

    if (argc <= 1)
        return 1;

    const auto entryScript = argv[1];
    auto entryRuntime = ScriptRuntime::createExternal();
    entryRuntime->executeFile(entryScript);
    
    DeviceSetup setup;
    setup.mode = static_cast<DeviceMode>(entryRuntime->getInt("deviceMode"));
    setup.canvasWidth = entryRuntime->getInt("canvasWidth");
    setup.canvasHeight = entryRuntime->getInt("canvasHeight");
    setup.logFilePath = entryRuntime->getString("logFilePath");
    const auto runScript = entryRuntime->getString("runScript");
    // TODO other fields

    const auto device = Device::create(setup);
    device->getScriptRuntime()->executeFile(runScript);
    
    return 0;
}