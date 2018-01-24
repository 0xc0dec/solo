/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloDevice.h"
#include "SoloDeviceSetup.h"
#include "SoloScriptRuntime.h"
#include "SoloComponent.h"

using namespace solo;

u32 ComponentTypeId::counter = 0;

int main(int argc, s8 *argv[])
{
    if (argc <= 1)
        return 1;

    try
    {
        const auto entryScript = argv[1];
        auto transientRuntime = ScriptRuntime::create();
        transientRuntime->executeFile(entryScript);

        const auto setup = transientRuntime->getDeviceSetup("deviceSetup");
        const auto runScript = transientRuntime->getString("runScript");

        const auto device = Device::create(setup);
        device->getScriptRuntime()->executeFile(runScript);
    }
    catch (const std::exception &e)
    {
		Logger::global().logCritical(e.what());
        return 2;
    }
    
    return 0;
}
