#include "SoloLuaScripter.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloNode.h"

#include <LuaBridge.h>

using namespace solo;


LuaScripter::LuaScripter()
{
	lua = luaL_newstate();
	luaL_openlibs(lua);
	registerScriptApi();
}


void LuaScripter::execString(const std::string& script)
{
	luaL_dostring(lua, script.c_str());
}


void LuaScripter::execFile(const std::string& scriptFileName)
{
	luaL_dofile(lua, scriptFileName.c_str());
}


lua_State* LuaScripter::getLuaState() const
{
	return lua;
}


void LuaScripter::registerScriptApi()
{
	using namespace luabridge;

	getGlobalNamespace(lua)
		.beginNamespace("solo")
			.beginClass<Device>("Device")
				.addFunction("setWindowTitle", &Device::setWindowTitle)
				.addFunction("getWindowTitle", &Device::getWindowTitle)
			.endClass()

			.beginClass<Node_ScriptWrap<Node>>("Node_ScriptWrap")
				.addFunction("addScript", &Node_ScriptWrap<Node>::addScript)
			.endClass()
			.deriveClass<Node, Node_ScriptWrap<Node>>("Node")
				.addFunction("getId", &Node::getId)
			.endClass()

			.beginClass<Scene>("Scene")
				.addFunction("createNode", &Scene::createNode)
			.endClass()

			.beginClass<Engine>("Engine")
				.addFunction("getDevice", &Engine::getDevice)
				.addFunction("getScene", &Engine::getScene)
			.endClass()

			.addVariable("engine", Engine::get(), false)
		.endNamespace();
}