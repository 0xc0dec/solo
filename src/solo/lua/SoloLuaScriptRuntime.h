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

            void executeFile(const std::string& path) override final;

            auto readString(const std::string &name) -> std::string override final;
            auto readDeviceSetup(const std::string &name) -> DeviceSetup override final;

        private:
            LuaIntf::LuaState lua;
        };
    }
}