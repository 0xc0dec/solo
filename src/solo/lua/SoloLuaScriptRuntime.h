/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloScriptRuntime.h"
#include "SoloLuaCommon.h"

namespace solo
{
    class LuaScriptRuntime final : public ScriptRuntime
    {
    public:
        LuaScriptRuntime();
        explicit LuaScriptRuntime(Device *device);
        ~LuaScriptRuntime();

        void executeFile(const str& path) override final;

	    auto eval(const str& code) -> str override;
        auto getString(const str &name) -> str override final;
        auto getDeviceSetup(const str &name) -> DeviceSetup override final;

    private:
        LuaState lua;
    };
}