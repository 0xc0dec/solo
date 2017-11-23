/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloScriptRuntime.h"
#include <LuaIntf.h>

namespace solo
{
    class LuaScriptRuntime final : public ScriptRuntime
    {
    public:
        LuaScriptRuntime();
        explicit LuaScriptRuntime(Device *device);
        ~LuaScriptRuntime();

        void executeFile(const str& path) override final;

        auto readString(const str &name) -> str override final;
        auto readDeviceSetup(const str &name) -> DeviceSetup override final;

    private:
        LuaIntf::LuaState lua;
    };
}