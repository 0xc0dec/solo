/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include <Solo.h>

int main()
{
    using namespace solo;

    // Run Lua tests
    DeviceSetup setup;
    setup.mode = DeviceMode::Null;
    auto device = Device::create(setup);
    device->getScriptRuntime()->executeFile("../../src/tests/tests.lua");

    // Run demo
    auto confRuntime = ScriptRuntime::createExternal();
    confRuntime->executeFile("../../src/demos/demo1/demo1.init.lua");

    setup.mode = static_cast<DeviceMode>(confRuntime->getInt("deviceMode"));
    setup.canvasWidth = confRuntime->getInt("canvasWidth");
    setup.canvasHeight = confRuntime->getInt("canvasHeight");
    setup.logFilePath = confRuntime->getString("logFilePath");
    
    device = Device::create(setup);
    device->getScriptRuntime()->executeFile("../../src/demos/demo1/demo1.lua");
    
    return 0;
}
