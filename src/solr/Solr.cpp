/*
 * Solr - Solo Runner, a standalone runner for Lua engine programs.
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
        const auto cfgScript = argv[1];
        auto transientRuntime = ScriptRuntime::empty();
        transientRuntime->execFile(cfgScript);

        const auto setup = transientRuntime->fetchDeviceSetup("setup");
        const auto runScript = transientRuntime->fetchString("entry");

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
