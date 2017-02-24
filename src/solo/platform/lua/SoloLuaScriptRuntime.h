/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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