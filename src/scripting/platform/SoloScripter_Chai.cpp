#include "SoloScripter_Chai.h"
#include "SoloScriptComponent_Chai.h"
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


Scripter_Chai::Scripter_Chai()
{
	engine = NEW<chaiscript::ChaiScript>(chaiscript::Std_Lib::library());
	registerScriptApi();
}


void Scripter_Chai::execString(const std::string& script)
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


void Scripter_Chai::execFile(const std::string& scriptFileName)
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


shared<chaiscript::ChaiScript> Scripter_Chai::getEngine() const
{
	return engine;
}


void Scripter_Chai::registerScriptApi()
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
	engine->add(user_type<Node>(), "Node");
	engine->add(fun(&Node::getId), "getId");
	engine->add(fun(&ScriptComponent_Chai::addComponent), "addComponent");
}