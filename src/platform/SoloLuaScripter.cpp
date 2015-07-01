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

	using namespace luabridge;

	getGlobalNamespace(lua)
		.beginNamespace("solo")
			.beginClass<Device>("Device")
				.addFunction("setWindowTitle", &Device::setWindowTitle)
				.addFunction("getWindowTitle", &Device::getWindowTitle)
			.endClass()

			.beginClass<ScriptedNode<Node>>("ScriptedNode")
				.addFunction("addScriptComponent", &ScriptedNode<Node>::addScriptComponent)
			.endClass()
			.deriveClass<Node, ScriptedNode<Node>>("Node")
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


void LuaScripter::execString(const std::string& script)
{
	luaL_dostring(lua, script.c_str());
}


void LuaScripter::execFile(const std::string& scriptFileName)
{
	luaL_dofile(lua, scriptFileName.c_str());
}
