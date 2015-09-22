#include <vector>
#include <map>
#include "SoloLuaScriptManager.h"
#include "SoloLuaScriptComponent.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloResourceManager.h"
#include "SoloNode.h"
#include "SoloVector3.h"

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


LuaScriptManager::~LuaScriptManager()
{
	lua.close();
}


void LuaScriptManager::registerApi()
{
	LuaBinding(lua).beginModule("solo")
		.beginClass<Vector3>("Vector3")
			.addConstructor(LUA_ARGS(float, float, float))
			.addStaticFunction("zero", &Vector3::zero)
			.addStaticFunction("unit", &Vector3::unit)
			.addStaticFunction("unitX", &Vector3::unitX)
			.addStaticFunction("unitY", &Vector3::unitY)
			.addStaticFunction("unitZ", &Vector3::unitZ)
			.addStaticFunction("angle", &Vector3::angle)
			.addStaticFunction("cross", &Vector3::cross)
			.addStaticFunction("dot", static_cast<float(*)(const Vector3&, const Vector3&)>(&Vector3::dot))
			.addFunction("distance", &Vector3::distance)
			.addFunction("distanceSquared", &Vector3::distanceSquared)
			.addFunction("dot", static_cast<float(Vector3::*)(const Vector3&)const>(&Vector3::dot))
			.addFunction("clamp", &Vector3::clamp)
			.addFunction("length", &Vector3::length)
			.addFunction("lengthSquared", &Vector3::lengthSquared)
			.addFunction("isUnit", &Vector3::isUnit)
			.addFunction("isZero", &Vector3::isZero)
			.addFunction("normalize", &Vector3::normalize)
			.addFunction("normalized", &Vector3::normalized)
			.addFunction("set", static_cast<void(Vector3::*)(float, float, float)>(&Vector3::set))
			.addFunction("set", static_cast<void(Vector3::*)(const Vector3&)>(&Vector3::set))
			.addVariable("x", &Vector3::x, true)
			.addVariable("y", &Vector3::y, true)
			.addVariable("z", &Vector3::z, true)
			.addFunction("plusScalar", static_cast<Vector3(Vector3::*)(float)const>(&Vector3::operator+))
			.addFunction("plusVector3", static_cast<Vector3(Vector3::*)(const Vector3&)const>(&Vector3::operator+))
			.addFunction("addScalar", static_cast<Vector3&(Vector3::*)(float)>(&Vector3::operator+=))
			.addFunction("addVector3", static_cast<Vector3&(Vector3::*)(const Vector3&)>(&Vector3::operator+=))
		.endClass()
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
			.addFunction("addComponent", LuaScriptComponent::addComponent)
			.addFunction("removeComponent", &LuaScriptComponent::removeComponent)
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


void LuaScriptManager::execute(const std::string& code)
{
	lua.doString(code.c_str());
}


void LuaScriptManager::executeFile(const std::string& path)
{
	lua.doFile(path.c_str());
}