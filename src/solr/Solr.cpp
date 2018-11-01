/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include <Solo.h>

using namespace solo;

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
        device->scriptRuntime()->executeFile(runScript);
    }
    catch (const std::exception &e)
    {
        Logger::global().logCritical(e.what());
        return 2;
    }
    
    return 0;
}
