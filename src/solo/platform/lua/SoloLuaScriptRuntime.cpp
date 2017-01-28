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
void registerDevice(CppBindModule<LuaBinding> &module);
void registerLogger(CppBindModule<LuaBinding> &module);
void registerMath(CppBindModule<LuaBinding> &module);
void registerScene(CppBindModule<LuaBinding> &module);
void registerNodeAndComponent(CppBindModule<LuaBinding> &module);
void registerTransform(CppBindModule<LuaBinding> &module);
void registerCamera(CppBindModule<LuaBinding> &module);
void registerRenderContext(CppBindModule<LuaBinding> &module);
void registerFrameBuffer(CppBindModule<LuaBinding> &module);
void registerTexture(CppBindModule<LuaBinding> &module);
void registerMeshRenderer(CppBindModule<LuaBinding> &module);
void registerEffect(CppBindModule<LuaBinding> &module);
void registerMaterial(CppBindModule<LuaBinding> &module);
void registerDeviceSetup(CppBindModule<LuaBinding> &module);
void registerFileSystem(CppBindModule<LuaBinding> &module);
void registerAssetLoader(CppBindModule<LuaBinding> &module);


LuaScriptRuntime::LuaScriptRuntime(Device *d)
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("solo");
    
    registerEnums(module);
    registerMath(module);
    registerDevice(module);
    registerLogger(module);
    registerScene(module);
    registerNodeAndComponent(module);
    registerTransform(module);
    registerCamera(module);
    registerRenderContext(module);
    registerFrameBuffer(module);
    registerTexture(module);
    registerMeshRenderer(module);
    registerEffect(module);
    registerMaterial(module);
    registerDeviceSetup(module);
    registerFileSystem(module);
    registerAssetLoader(module);

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
