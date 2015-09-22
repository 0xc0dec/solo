#include <vector>
#include <map>
#include "SoloLuaScriptManager.h"
#include "SoloEngine.h"
#include "SoloDevice.h"

namespace LuaIntf
{
	LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
	LUA_USING_LIST_TYPE(std::vector)
	LUA_USING_MAP_TYPE(std::map)
}


using namespace solo;
using namespace LuaIntf;


LuaScriptManager::LuaScriptManager(Engine *engine):
	engine(engine)
{
	lua = LuaState::newState();
	lua.openLibs();
	registerApi();
}


void LuaScriptManager::registerApi()
{
	LuaBinding(lua).beginModule("solo")
		.beginClass<Device>("Device")
			.addFunction("getWindowTitle", &Device::getWindowTitle)
			.addFunction("setWindowTitle", &Device::setWindowTitle)
		.endClass()
		.beginClass<Engine>("Engine")
			.addFunction("getDevice", &Engine::getDevice)
		.endClass()
		.addVariable("engine", engine, false)
	.endModule();
}


LuaScriptManager::~LuaScriptManager()
{
	lua.close();
}


void LuaScriptManager::execute(const std::string& code)
{
	lua.doString(code.c_str());
}


void LuaScriptManager::executeFile(const std::string& path)
{
	lua.doFile(path.c_str());
}