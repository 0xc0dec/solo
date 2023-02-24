/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloScriptRuntime.h"
#include "SoloLuaCommon.h"

namespace solo {
    class LuaScriptRuntime final : public ScriptRuntime {
    public:
        LuaScriptRuntime();
        explicit LuaScriptRuntime(Device *device);
        ~LuaScriptRuntime();

        void execFile(const str &path) override final;
        auto eval(const str &code) -> str override;

        auto fetchDeviceSetup(const str &name) -> DeviceSetup override final;
        auto fetchString(const str &name) -> str override final;

    private:
        LuaState lua_;
    };
}