/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include <Solo.h>

int main()
{
    using namespace solo;

    auto confRuntime = ScriptRuntime::createExternal();
    confRuntime->executeFile("../../src/demos/demo2/Demo2.init.lua");

    DeviceSetup setup;
    setup.mode = static_cast<DeviceMode>(confRuntime->getInt("deviceMode"));
    setup.canvasWidth = confRuntime->getInt("canvasWidth");
    setup.canvasHeight = confRuntime->getInt("canvasHeight");
    setup.logFilePath = confRuntime->getString("logFilePath");
    
    auto device = Device::create(setup);
    device->getScriptRuntime()->executeFile("../../src/demos/demo2/Demo2.lua");
    
    return 0;
}
