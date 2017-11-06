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

u32 ComponentTypeId::counter = 0;

int main(int argc, s8 *argv[])
{
    if (argc <= 1)
        return 1;

    try
    {
        const auto entryScript = argv[1];
        auto entryRuntime = ScriptRuntime::create();
        entryRuntime->executeFile(entryScript);

        const auto setup = entryRuntime->readDeviceSetup("deviceSetup");
        const auto runScript = entryRuntime->readString("runScript");

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
