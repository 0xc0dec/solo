/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
void registerLibrary(LuaState &state);

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

LuaScriptRuntime::LuaScriptRuntime()
{
    lua_ = LuaState::newState();
    lua_.openLibs();

    auto module = LuaBinding(lua_).beginModule("sl");
    registerApi(module);
    registerLibrary(lua_);
    module.endModule();
}

LuaScriptRuntime::LuaScriptRuntime(Device *device):
    LuaScriptRuntime()
{
    auto module = LuaBinding(lua_).beginModule("sl");
    module.addConstant("device", device);
    module.endModule();
}

LuaScriptRuntime::~LuaScriptRuntime()
{
    lua_.close();
}

void LuaScriptRuntime::execFile(const str& path)
{
    if (lua_.loadFile(path.c_str()))
    {
        const auto msg = lua_.getString(-1);
    	Logger::global().logError(fmt("Script failed to load: ", msg));
    }

    // TODO use FileSystem to read the file
    lua_.doFile(path.c_str());
}

auto LuaScriptRuntime::eval(const str& code) -> str
{
    return lua_.eval<str>(code.c_str());
}

auto LuaScriptRuntime::fetchString(const str& name) -> str
{
    return LuaRef(lua_, name.c_str()).toValue<str>();
}

auto LuaScriptRuntime::fetchDeviceSetup(const str &name) -> DeviceSetup
{
    return LuaRef(lua_, name.c_str()).toValue<DeviceSetup>();
}
