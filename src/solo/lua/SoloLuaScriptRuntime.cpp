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
void registerApi(sol::table &module);

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

        sl.createComponent = function(id, shape)
            shape.typeId = sl.getCmpId(id)
            return shape
        end
    )");
}

LuaScriptRuntime::LuaScriptRuntime()
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("sl");
    registerApi(module);
    registerLibrary(lua);
    module.endModule();
}

LuaScriptRuntime::LuaScriptRuntime(Device *device):
    LuaScriptRuntime()
{
    auto module = LuaBinding(lua).beginModule("sl");
    module.addConstant("device", device);
    module.endModule();
}

LuaScriptRuntime::~LuaScriptRuntime()
{
    lua.close();
}

void LuaScriptRuntime::executeFile(const str& path)
{
    if (lua.loadFile(path.c_str()))
    {
        const auto msg = lua.getString(-1);
        SL_PANIC(SL_FMT("Script failed to load: ", msg));
    }

    // TODO use FileSystem to read the file
    lua.doFile(path.c_str());
}

auto LuaScriptRuntime::readString(const str& name) -> str
{
    return readValue<str>(lua, name);
}

auto LuaScriptRuntime::readDeviceSetup(const str &name) -> DeviceSetup
{
    return readValue<DeviceSetup>(lua, name);
}

LuaScriptRuntime2::LuaScriptRuntime2()
{
	state.open_libraries();

	auto t = state.create_table("sl");
	registerApi(t);
}

LuaScriptRuntime2::LuaScriptRuntime2(Device* device):
	LuaScriptRuntime2()
{
	state["sl"]["device"] = device;
}

void LuaScriptRuntime2::executeFile(const str& path)
{
	state.do_file(path);
}

auto LuaScriptRuntime2::readString(const str& name) -> str
{
	return state[name].get<str>();
}

auto LuaScriptRuntime2::readDeviceSetup(const str& name) -> DeviceSetup
{
	return state[name].get<DeviceSetup>();
}
