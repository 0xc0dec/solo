#include "SoloChaiScriptScripter.h"
#include "SoloChaiScriptScript.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloNode.h"
#include "SoloResourceManager.h"
#include "SoloTexture.h"
#include "SoloFileSystem.h"
#include "SoloNode.h"
#include <chaiscript.hpp>
#include <chaiscript_stdlib.hpp>

using namespace solo;


ChaiScriptScripter::ChaiScriptScripter()
{
	engine = NEW<chaiscript::ChaiScript>(chaiscript::Std_Lib::library());
	registerScriptApi();
}


void ChaiScriptScripter::execString(const std::string& script)
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


void ChaiScriptScripter::execFile(const std::string& scriptFileName)
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


shared<chaiscript::ChaiScript> ChaiScriptScripter::getEngine() const
{
	return engine;
}


ChaiScriptScript* addComponent(chaiscript::Boxed_Value& node, chaiscript::Boxed_Value component)
{
	auto n = chaiscript::boxed_cast<Node*>(node);
	auto script = ScriptFactory_Chai::create(n, component);
	Engine::get()->getScene()->addComponent(n, script);
	return script.get();
}


void ChaiScriptScripter::registerScriptApi()
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
//	engine->add(user_type<Node_ScriptWrap<Node>>(), "Node_ScriptWrap");
	engine->add(user_type<Node>(), "Node");
//	engine->add(base_class<Node_ScriptWrap<Node>, Node>());
	engine->add(fun(&Node::getId), "getId");
	engine->add(fun(&addComponent), "addComponent");
//	engine->add(fun(&Node_ScriptWrap<Node>::addComponent), "addComponent");
//	engine->add(fun(&Node_ScriptWrap<Node>::removeComponent), "removeComponent");

	// Script
//	engine->add(user_type<Script>(), "Script");
//	engine->add(user_type<ChaiScriptScript>(), "ChaiScriptScript");
//	engine->add(base_class<Script, ChaiScriptScript>());
//	engine->add(fun(&ChaiScriptScript::setUpdateCallback), "setUpdateCallback");
}