/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <functional>

namespace solo
{
    class Device;

    class ScriptRuntime
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(ScriptRuntime)

        static auto createExternal() -> sptr<ScriptRuntime>;
        static auto create(Device *device, const FriendToken<Device> &) -> sptr<ScriptRuntime>;

        ScriptRuntime() {}
        virtual ~ScriptRuntime() {}

        virtual void executeFile(const std::string& path) = 0;

        virtual auto getString(const std::string &name) -> std::string = 0;
        virtual auto getInt(const std::string &name) -> int32_t = 0;
    };
}