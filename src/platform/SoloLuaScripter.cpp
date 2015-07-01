#include "SoloLuaScripter.h"
#include "SoloEngine.h"
#include "../scripting/SoloDevice_ScriptWrap.h"
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
			.endClass()

			.beginClass<Engine>("Engine")
			.addFunction("getDevice", &Engine::getDevice)
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
