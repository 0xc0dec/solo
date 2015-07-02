#include "SoloLuaScript.h"
#include "SoloEngine.h"
#include "SoloLuaScripter.h"
#include "SoloDevice.h"
#include <LuaBridge.h>

using namespace solo;


LuaScript::LuaScript(Node* node, const std::string& callbackObjectName) :
	Script(node),
	callbackObjectName(callbackObjectName)
{
	auto engine = Engine::get();
//	lua = static_cast<LuaScripter*>(engine->getScripter())->getLuaState();
	device = engine->getDevice();
}


void LuaScript::update()
{
//	auto callback = luabridge::getGlobal(lua, callbackObjectName.c_str());
//	callback["update"](device->getTimeDelta());
}