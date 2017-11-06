/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloScriptRuntime.h"
#include <LuaIntf.h>

namespace solo
{
    namespace lua
    {
        class ScriptRuntime final : public solo::ScriptRuntime
        {
        public:
            ScriptRuntime();
            explicit ScriptRuntime(Device *device);
            ~ScriptRuntime();

            void executeFile(const str& path) override final;

            auto readString(const str &name) -> str override final;
            auto readDeviceSetup(const str &name) -> DeviceSetup override final;

        private:
            LuaIntf::LuaState lua;
        };
    }
}