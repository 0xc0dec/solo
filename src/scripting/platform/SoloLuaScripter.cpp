#include "SoloLuaScripter.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloNode.h"
#include "SoloResourceManager.h"
#include "SoloTexture.h"
#include "SoloFileSystem.h"
#include "SoloNode.h"
#include "SoloLuaScript.h"
#include <chaiscript.hpp>
#include <chaiscript_stdlib.hpp>

using namespace solo;


LuaScripter::LuaScripter()
{
	engine = NEW<chaiscript::ChaiScript>(chaiscript::Std_Lib::library());
	registerScriptApi();
}


void LuaScripter::execString(const std::string& script)
{
	try
	{
		engine->eval(script);
	}
	catch (chaiscript::exception::eval_error &e)
	{
		THROW_FMT(EngineException, e.pretty_print());
	}
}


void LuaScripter::execFile(const std::string& scriptFileName)
{
	try
	{
		engine->eval_file(scriptFileName);
	}
	catch (chaiscript::exception::eval_error &e)
	{
		THROW_FMT(EngineException, e.pretty_print());
	}
}


chaiscript::ChaiScript* LuaScripter::getScriptEngine() const
{
	return engine.get();
}


void LuaScripter::registerScriptApi()
{
	using namespace chaiscript;

	// Engine
	engine->add(user_type<Engine>(), "Engine");
	engine->add(fun(&Engine::getDevice), "getDevice");
	engine->add(fun(&Engine::getFileSystem), "getFileSystem");
	engine->add(fun(&Engine::getResourceManager), "getResourceManager");
	engine->add(fun(&Engine::getScene), "getScene");
	engine->add(var(Engine::get()), "engine");

	// Device
	engine->add(user_type<Device>(), "Device");
	engine->add(fun(&Device::getWindowTitle), "getWindowTitle");
	engine->add(fun(&Device::setWindowTitle), "setWindowTitle");

	// File system
	engine->add(user_type<FileSystem>(), "FileSystem");

	// Resource manager
	engine->add(user_type<ResourceManager>(), "ResourceManager");

	// Scene
	engine->add(user_type<Scene>(), "Scene");
	engine->add(fun(&Scene::createNode), "createNode");

	// Node
	engine->add(user_type<Node_ScriptWrap<Node>>(), "Node_ScriptWrap");
	engine->add(user_type<Node>(), "Node");
	engine->add(base_class<Node_ScriptWrap<Node>, Node>());
	engine->add(fun(&Node::getId), "getId");
	engine->add(fun(&Node_ScriptWrap<Node>::addScript), "addScript");

	// LuaScript
	engine->add(user_type<Script>(), "Script");
	engine->add(user_type<LuaScript>(), "LuaScript");
	engine->add(base_class<Script, LuaScript>());
	engine->add(fun(&LuaScript::setUpdateCallback), "setUpdateCallback");
}


template <typename T> T LuaScripter::eval(const std::string& code)
{
	return engine->eval<T>(code);
}