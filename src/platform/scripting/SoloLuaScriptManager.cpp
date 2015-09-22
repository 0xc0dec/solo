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


void registerVector3(CppBindModule& module)
{
	module.beginClass<Vector3>("Vector3")
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
		.addFunction("assign", static_cast<void(Vector3::*)(const Vector3&)>(&Vector3::set))
		.addVariable("x", &Vector3::x, true)
		.addVariable("y", &Vector3::y, true)
		.addVariable("z", &Vector3::z, true)
		.addFunction("plusScalar", static_cast<Vector3(Vector3::*)(float)const>(&Vector3::operator+))
		.addFunction("plusVector3", static_cast<Vector3(Vector3::*)(const Vector3&)const>(&Vector3::operator+))
		.addFunction("addScalar", static_cast<Vector3&(Vector3::*)(float)>(&Vector3::operator+=))
		.addFunction("addVector3", static_cast<Vector3&(Vector3::*)(const Vector3&)>(&Vector3::operator+=))
	.endClass();
}


void registerVector2(CppBindModule& module)
{
	module.beginClass<Vector2>("Vector2")
		.addConstructor(LUA_ARGS(float, float))
		.addStaticFunction("zero", &Vector2::zero)
		.addStaticFunction("unit", &Vector2::unit)
		.addStaticFunction("unitX", &Vector2::unitX)
		.addStaticFunction("unitY", &Vector2::unitY)
		.addStaticFunction("angle", &Vector2::angle)
		.addStaticFunction("dot", static_cast<float(*)(const Vector2&, const Vector2&)>(&Vector2::dot))
		.addFunction("distance", &Vector2::distance)
		.addFunction("distanceSquared", &Vector2::distanceSquared)
		.addFunction("dot", static_cast<float(Vector2::*)(const Vector2&)const>(&Vector2::dot))
		.addFunction("clamp", &Vector2::clamp)
		.addFunction("length", &Vector2::length)
		.addFunction("lengthSquared", &Vector2::lengthSquared)
		.addFunction("isUnit", &Vector2::isUnit)
		.addFunction("isZero", &Vector2::isZero)
		.addFunction("normalize", &Vector2::normalize)
		.addFunction("normalized", &Vector2::normalized)
		.addFunction("set", static_cast<void(Vector2::*)(float, float)>(&Vector2::set))
		.addFunction("assign", static_cast<void(Vector2::*)(const Vector2&)>(&Vector2::set))
		.addVariable("x", &Vector2::x, true)
		.addVariable("y", &Vector2::y, true)
		.addFunction("plusScalar", static_cast<Vector2(Vector2::*)(float)const>(&Vector2::operator+))
		.addFunction("plusVector2", static_cast<Vector2(Vector2::*)(const Vector2&)const>(&Vector2::operator+))
		.addFunction("addScalar", static_cast<Vector2&(Vector2::*)(float)>(&Vector2::operator+=))
		.addFunction("addVector2", static_cast<Vector2&(Vector2::*)(const Vector2&)>(&Vector2::operator+=))
	.endClass();
}


void registerDevice(CppBindModule& module)
{
	module.beginClass<Device>("Device")
		.addFunction("getWindowTitle", &Device::getWindowTitle)
		.addFunction("setWindowTitle", &Device::setWindowTitle)
		.addFunction("getLifetime", &Device::getLifetime)
	.endClass();
}


void registerResourceManager(CppBindModule& module)
{
	module.beginClass<ResourceManager>("ResourceManager")
		// TODO
	.endClass();
}


void registerNode(CppBindModule& module)
{
	module.beginClass<Node>("Node")
		.addFunction("getScene", &Node::getScene)
		.addFunction("getId", &Node::getId)
		.addFunction("addComponent", LuaScriptComponent::addComponent)
		.addFunction("removeComponent", &LuaScriptComponent::removeComponent)
		.addFunction("removeAllComponents", &Node::removeAllComponents)
	.endClass();
}


void registerScene(CppBindModule& module)
{
	module.beginClass<Scene>("Scene")
		.addFunction("createNode", &Scene::createNode)
	.endClass();
}


void registerEngine(CppBindModule& module)
{
	module.beginClass<Engine>("Engine")
		.addFunction("getDevice", &Engine::getDevice)
		.addFunction("getScene", &Engine::getScene)
	.endClass();
}


void LuaScriptManager::registerApi()
{
	auto module = LuaBinding(lua).beginModule("solo");
	registerVector2(module);
	registerVector3(module);
	registerDevice(module);
	registerResourceManager(module);
	registerNode(module);
	registerScene(module);
	registerEngine(module);
	module.addVariable("engine", engine, false);
	module.endModule();
}


void LuaScriptManager::execute(const std::string& code)
{
	lua.doString(code.c_str());
}


void LuaScriptManager::executeFile(const std::string& path)
{
	lua.doFile(path.c_str());
}