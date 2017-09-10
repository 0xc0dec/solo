/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloDevice.h"
#include "SoloDeviceSetup.h"
#include "SoloScriptRuntime.h"
#include "SoloComponent.h"
#include <iostream>

using namespace solo;

uint32_t ComponentTypeId::counter = 0;

int main(int argc, char **argv)
{
    if (argc <= 1)
        return 1;

    try
    {
        const auto entryScript = argv[1];
        auto entryRuntime = ScriptRuntime::create();
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
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 2;
    }
    
    return 0;
}