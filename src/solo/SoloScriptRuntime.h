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

    class ScriptRuntime
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(ScriptRuntime)

        static auto create() -> sptr<ScriptRuntime>;
        static auto create(Device *device, const FriendToken<Device> &) -> sptr<ScriptRuntime>;

        ScriptRuntime() {}
        virtual ~ScriptRuntime() {}

        virtual void executeFile(const std::string& path) = 0;

        virtual auto readString(const std::string &name) -> std::string = 0;
        virtual auto readDeviceSetup(const std::string &name) -> DeviceSetup = 0;
    };
}