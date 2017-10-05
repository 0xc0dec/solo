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
        sl.getCmpId = function(name)
            sl.__nextCmpId = sl.__nextCmpId and sl.__nextCmpId or 1
            sl.__cmpIds = sl.__cmpIds and sl.__cmpIds or {}
    
            local id = sl.__cmpIds[name]
            if not id then
                id = sl.__nextCmpId
                sl.__cmpIds[name] = id
                sl.__nextCmpId = sl.__nextCmpId + 1
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

    auto module = LuaBinding(lua).beginModule("sl");
    registerApi(module);
    registerLibrary(lua);
    module.endModule();
}

lua::ScriptRuntime::ScriptRuntime(Device *d):
    ScriptRuntime()
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("sl");
    registerApi(module);
    registerLibrary(lua);
    module.addConstant("device", d);
    module.endModule();
}

lua::ScriptRuntime::~ScriptRuntime()
{
    lua.close();
}

void lua::ScriptRuntime::executeFile(const std::string& path)
{
    if (lua.loadFile(path.c_str()))
    {
        auto msg = lua.getString(-1);
        SL_PANIC(SL_FMT("Script failed to load: ", msg));
    }

    // TODO use FileSystem to read the file
    lua.doFile(path.c_str());
}

auto lua::ScriptRuntime::readString(const std::string& name) -> std::string
{
    auto ref = LuaRef(lua, name.c_str());
    return ref.toValue<std::string>();
}

auto lua::ScriptRuntime::readDeviceSetup(const std::string &name) -> DeviceSetup
{
    auto ref = LuaRef(lua, name.c_str());
    return ref.toValue<DeviceSetup>();
}
