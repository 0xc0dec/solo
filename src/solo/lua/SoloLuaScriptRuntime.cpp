/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
lua::ScriptRuntime::ScriptRuntime()
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("solo");
    registerApi(module);
    registerLibrary(lua);
    module.endModule();
}

lua::ScriptRuntime::ScriptRuntime(Device *d):
    ScriptRuntime()
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("solo");
    registerApi(module);
    registerLibrary(lua);
    module.addConstant("device", d);
    module.endModule();
}

lua::ScriptRuntime::~ScriptRuntime()
{
    lua.close();
}

void lua::ScriptRuntime::executeString(const std::string& code)
{
    lua.doString(code.c_str());
}

void lua::ScriptRuntime::executeFile(const std::string& path)
{
    lua.doFile(path.c_str());
}

auto lua::ScriptRuntime::getString(const std::string& name) -> std::string
{
    auto ref = LuaRef(lua, name.c_str());
    return ref.toValue<std::string>();
}

auto lua::ScriptRuntime::getInt(const std::string& name) -> int32_t
{
    auto ref = LuaRef(lua, name.c_str());
    return ref.toValue<int32_t>();
}
