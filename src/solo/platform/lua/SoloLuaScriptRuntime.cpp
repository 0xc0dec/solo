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

#include "SoloLuaScriptRuntime.h"
#include "SoloDevice.h"
#include "SoloLogger.h"
#include "SoloLuaCommon.h"
#include <map>
#include <vector>

using namespace solo;
using namespace LuaIntf;

namespace LuaIntf
{
    LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
    LUA_USING_LIST_TYPE(std::vector)
    LUA_USING_MAP_TYPE(std::map) // TODO unordered_map
}


LuaScriptRuntime::LuaScriptRuntime(Device *d)
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("solo");
    
    registerMath(module);
    registerDevice(module);
    registerLogger(module);

    module.addConstant("device", d);

    module.endModule();
}


LuaScriptRuntime::~LuaScriptRuntime()
{
    lua.close();
}


void LuaScriptRuntime::executeString(const std::string& code)
{
    lua.doString(code.c_str());
}


void LuaScriptRuntime::executeFile(const std::string& path)
{
    lua.doFile(path.c_str());
}
