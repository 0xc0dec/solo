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
	engine->add(fun(&Device::requestShutdown), "requestShutdown");
	engine->add(fun(&Device::shutdownRequested), "shutdownRequested");
	engine->add(fun(&Device::getMouseMotion), "getMouseMotion");
	engine->add(fun(&Device::setCursorCaptured), "setCursorCaptured");
	engine->add(fun(&Device::getCanvasSize), "getCanvasSize");
	engine->add(fun(&Device::getLifetime), "getLifetime");
	engine->add(fun(&Device::getTimeDelta), "getTimeDelta");
	engine->add(fun(&Device::isKeyPressed), "isKeyPressed");
	engine->add(fun(&Device::isKeyReleased), "isKeyReleased");

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
	engine->add(fun(&ScriptComponent_Chai::removeComponent), "removeComponent");
	engine->add(fun(&ScriptComponent_Chai::findComponent), "findComponent");

	// Vector2
	engine->add(user_type<Vector2>(), "Vector2");
	engine->add(fun(&Vector2::x), "x");
	engine->add(fun(&Vector2::y), "y");

	engine->add(user_type<KeyCode>(), "KeyCode");
	engine->add(const_var(KeyCode::A), "KeyCode_A");
	engine->add(const_var(KeyCode::B), "KeyCode_B");
	engine->add(const_var(KeyCode::C), "KeyCode_C");
	engine->add(const_var(KeyCode::D), "KeyCode_D");
	engine->add(const_var(KeyCode::E), "KeyCode_E");
	engine->add(const_var(KeyCode::F), "KeyCode_F");
	engine->add(const_var(KeyCode::G), "KeyCode_G");
	engine->add(const_var(KeyCode::H), "KeyCode_H");
	engine->add(const_var(KeyCode::I), "KeyCode_I");
	engine->add(const_var(KeyCode::J), "KeyCode_J");
	engine->add(const_var(KeyCode::K), "KeyCode_K");
	engine->add(const_var(KeyCode::L), "KeyCode_L");
	engine->add(const_var(KeyCode::M), "KeyCode_M");
	engine->add(const_var(KeyCode::N), "KeyCode_N");
	engine->add(const_var(KeyCode::O), "KeyCode_O");
	engine->add(const_var(KeyCode::P), "KeyCode_P");
	engine->add(const_var(KeyCode::Q), "KeyCode_Q");
	engine->add(const_var(KeyCode::R), "KeyCode_R");
	engine->add(const_var(KeyCode::S), "KeyCode_S");
	engine->add(const_var(KeyCode::T), "KeyCode_T");
	engine->add(const_var(KeyCode::U), "KeyCode_U");
	engine->add(const_var(KeyCode::V), "KeyCode_V");
	engine->add(const_var(KeyCode::W), "KeyCode_W");
	engine->add(const_var(KeyCode::X), "KeyCode_X");
	engine->add(const_var(KeyCode::Y), "KeyCode_Y");
	engine->add(const_var(KeyCode::Z), "KeyCode_Z");
	engine->add(const_var(KeyCode::UpArrow), "KeyCode_UpArrow");
	engine->add(const_var(KeyCode::DownArrow), "KeyCode_DownArrow");
	engine->add(const_var(KeyCode::LeftArrow), "KeyCode_LeftArrow");
	engine->add(const_var(KeyCode::RightArrow), "KeyCode_RightArrow");
	engine->add(const_var(KeyCode::Escape), "KeyCode_Escape");
}