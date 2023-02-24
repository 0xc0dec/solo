/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloScriptRuntime.h"
#include "SoloDevice.h"
#include "lua/SoloLuaScriptRuntime.h"

using namespace solo;

auto ScriptRuntime::empty() -> sptr<ScriptRuntime> {
    return std::make_shared<LuaScriptRuntime>();
}

auto ScriptRuntime::fromDevice(Device *device) -> sptr<ScriptRuntime> {
    return std::make_shared<LuaScriptRuntime>(device);
}
