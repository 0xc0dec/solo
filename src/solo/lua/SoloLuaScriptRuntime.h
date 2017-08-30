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

            void executeString(const std::string& code) override final;
            void executeFile(const std::string& path) override final;

            auto getString(const std::string &name) -> std::string override final;
            auto getInt(const std::string &name) -> int32_t override final;

        private:
            LuaIntf::LuaState lua;
        };
    }
}