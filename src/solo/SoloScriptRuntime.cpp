/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloScriptRuntime.h"
#include "SoloDevice.h"
#include "lua/SoloLuaScriptRuntime.h"

using namespace solo;

auto ScriptRuntime::empty() -> sptr<ScriptRuntime>
{
    return std::make_shared<LuaScriptRuntime>();
}

auto ScriptRuntime::withDevice(Device *device, const FriendToken<Device>&) -> sptr<ScriptRuntime>
{
    return std::make_shared<LuaScriptRuntime>(device);
}
