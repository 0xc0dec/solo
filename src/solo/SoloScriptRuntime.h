/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloDeviceSetup.h"
#include <functional>

namespace solo
{
    class Device;

    class ScriptRuntime: public NoCopyAndMove
    {
    public:
        static auto create() -> sptr<ScriptRuntime>;
        static auto create(Device *device, const FriendToken<Device> &) -> sptr<ScriptRuntime>;

        ScriptRuntime() = default;
        virtual ~ScriptRuntime() = default;

        virtual void execFile(const str& path) = 0;
        virtual auto eval(const str &code) -> str = 0;

        virtual auto fetchDeviceSetup(const str &name) -> DeviceSetup = 0;
        virtual auto fetchString(const str &name) -> str = 0;
    };
}