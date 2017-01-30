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
#include "SoloLuaCommon.h"

using namespace solo;


void registerEnums(CppBindModule<LuaBinding> &module);
void registerMathApi(CppBindModule<LuaBinding> &module);
void registerNodeAndComponentApi(CppBindModule<LuaBinding> &module);
void registerTransformApi(CppBindModule<LuaBinding> &module);
void registerCameraApi(CppBindModule<LuaBinding> &module);
void registerTextureApi(CppBindModule<LuaBinding> &module);
void registerMaterialApi(CppBindModule<LuaBinding> &module);
void registerMiscApi(CppBindModule<LuaBinding> &module);
void registerDeviceApi(CppBindModule<LuaBinding> &module);
void registerPhysicsApi(CppBindModule<LuaBinding> &module);
void registerMeshApi(CppBindModule<LuaBinding> &module);
void registerFontApi(CppBindModule<LuaBinding> &module);


static void registerApi(CppBindModule<LuaBinding> &module)
{
    registerEnums(module);
    registerMathApi(module);
    registerNodeAndComponentApi(module);
    registerTransformApi(module);
    registerCameraApi(module);
    registerTextureApi(module);
    registerMaterialApi(module);
    registerMiscApi(module);
    registerDeviceApi(module);
    registerPhysicsApi(module);
    registerMeshApi(module);
    registerFontApi(module);
}


static void registerLibrary(LuaState &state)
{
    state.doString(R"(
        solo.getCmpId = function(name)
            solo.__nextCmpId = solo.__nextCmpId and solo.__nextCmpId or 1
            solo.__cmpIds = solo.__cmpIds and solo.__cmpIds or {}
    
            local id = solo.__cmpIds[name]
            if not id then
                id = solo.__nextCmpId
                solo.__cmpIds[name] = id
                solo.__nextCmpId = solo.__nextCmpId + 1
            end

            return id
        end
    )");
}

// TODO remove copy-paste
LuaScriptRuntime::LuaScriptRuntime()
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("solo");
    registerApi(module);
    registerLibrary(lua);
    module.endModule();
}


LuaScriptRuntime::LuaScriptRuntime(Device *d):
    LuaScriptRuntime()
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("solo");
    registerApi(module);
    registerLibrary(lua);
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


auto LuaScriptRuntime::getString(const std::string& name) -> std::string
{
    auto ref = LuaRef(lua, name.c_str());
    return ref.toValue<std::string>();
}


auto LuaScriptRuntime::getInt(const std::string& name) -> int32_t
{
    auto ref = LuaRef(lua, name.c_str());
    return ref.toValue<int32_t>();
}
