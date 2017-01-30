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

        virtual void executeString(const std::string& code) = 0;
        virtual void executeFile(const std::string& path) = 0;

        virtual auto getString(const std::string &name) -> std::string = 0;
        virtual auto getInt(const std::string &name) -> int32_t = 0;
    };
}