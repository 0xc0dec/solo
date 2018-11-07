/*
 * Solr - Solo Runner, a standalone engine runner for script-only engine programs.
 * 
 * Copyright (c) Aleksey Fedotov
 * MIT license
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
        auto transientRuntime = ScriptRuntime::empty();
        transientRuntime->execFile(entryScript);

        const auto setup = transientRuntime->fetchDeviceSetup("deviceSetup");
        const auto runScript = transientRuntime->fetchString("runScript");

        const auto device = Device::create(setup);
        device->scriptRuntime()->execFile(runScript);
    }
    catch (const std::exception &e)
    {
        Logger::global().logCritical(e.what());
        return 2;
    }
    
    return 0;
}
