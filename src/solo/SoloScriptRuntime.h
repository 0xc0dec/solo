/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"
#include "SoloDeviceSetup.h"
#include <functional>

namespace solo
{
    class Device;

    class ScriptRuntime
    {
    public:
        static auto empty() -> sptr<ScriptRuntime>;
        static auto fromDevice(Device *device) -> sptr<ScriptRuntime>;

        ScriptRuntime(const ScriptRuntime &other) = delete;
        ScriptRuntime(ScriptRuntime &&other) = delete;
        virtual ~ScriptRuntime() = default;

        auto operator=(const ScriptRuntime &other) -> ScriptRuntime & = delete;
        auto operator=(ScriptRuntime &&other) -> ScriptRuntime & = delete;

        virtual void execFile(const str &path) = 0;
        virtual auto eval(const str &code) -> str = 0;

        virtual auto fetchDeviceSetup(const str &name) -> DeviceSetup = 0;
        virtual auto fetchString(const str &name) -> str = 0;

    protected:
        ScriptRuntime() = default;
    };
}