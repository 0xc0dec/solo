#include <vector>
#include <map>
#include "SoloLuaScriptManager.h"
#include "SoloLuaScriptComponent.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloResourceManager.h"
#include "SoloNode.h"

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
			.addFunction("getLifetime", &Device::getLifetime)
		.endClass()
		.beginClass<ResourceManager>("ResourceManager")
			// TODO
		.endClass()
		.beginClass<Node>("Node")
			.addFunction("getScene", &Node::getScene)
			.addFunction("getId", &Node::getId)
			.addFunction("addComponent", LuaScriptComponent::getAddComponentFunc(lua))
			.addFunction("removeAllComponents", &Node::removeAllComponents)
		.endClass()
		.beginClass<Scene>("Scene")
			.addFunction("createNode", &Scene::createNode)
		.endClass()
		.beginClass<Engine>("Engine")
			.addFunction("getDevice", &Engine::getDevice)
			.addFunction("getScene", &Engine::getScene)
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