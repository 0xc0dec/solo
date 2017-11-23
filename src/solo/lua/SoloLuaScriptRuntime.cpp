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

template <class T>
static auto readValue(LuaIntf::LuaState &lua, const str &name) -> T
{
    auto ref = LuaRef(lua, name.c_str());
    return ref.toValue<T>();
}

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

lua::LuaScriptRuntime::LuaScriptRuntime()
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("sl");
    registerApi(module);
    registerLibrary(lua);
    module.endModule();
}

lua::LuaScriptRuntime::LuaScriptRuntime(Device *d):
    LuaScriptRuntime()
{
    auto module = LuaBinding(lua).beginModule("sl");
    module.addConstant("device", d);
    module.endModule();
}

lua::LuaScriptRuntime::~LuaScriptRuntime()
{
    lua.close();
}

void lua::LuaScriptRuntime::executeFile(const str& path)
{
    if (lua.loadFile(path.c_str()))
    {
        auto msg = lua.getString(-1);
        SL_PANIC(SL_FMT("Script failed to load: ", msg));
    }

    // TODO use FileSystem to read the file
    lua.doFile(path.c_str());
}

auto lua::LuaScriptRuntime::readString(const str& name) -> str
{
    return readValue<str>(lua, name);
}

auto lua::LuaScriptRuntime::readDeviceSetup(const str &name) -> DeviceSetup
{
    return readValue<DeviceSetup>(lua, name);
}
