#include "SoloLuaScripter.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloNode.h"
#include "SoloResourceManager.h"
#include "SoloTexture.h"

//#include <SLB3/slb.h>
#include <LuaBridge.h>
#include <oolua.h>
#include <chaiscript.hpp>
#include <chaiscript_stdlib.hpp>

using namespace solo;


LuaScripter::LuaScripter()
{
	lua = luaL_newstate();
	luaL_openlibs(lua);
//	script = new OOLUA::Script();
	registerScriptApi();
}


void LuaScripter::execString(const std::string& script)
{
//	luaL_dostring(lua, script.c_str());
//	this->script->run_chunk(script);
}


void LuaScripter::execFile(const std::string& scriptFileName)
{
//	luaL_dofile(lua, scriptFileName.c_str());
//	this->script->run_file(scriptFileName);
}


lua_State* LuaScripter::getLuaState() const
{
	return lua;
}


class Lol
{
public:
	int getInt()
	{
		return 100;
	}
};


class Test
{
public:
	int getInt()
	{
		return 10;
	}

	std::string getString()
	{
		return "Some string";
	}

	std::string acceptString(const std::string& s)
	{
		return s;
	}

	std::shared_ptr<Lol> getPtr()
	{
		return std::make_shared<Lol>();
	}
};

//
//OOLUA_PROXY(Test)
//	OOLUA_MFUNC(getInt)
//	OOLUA_MFUNC(getString)
//	OOLUA_MFUNC(acceptString)
//	OOLUA_MFUNC(getPtr)
//OOLUA_PROXY_END
//OOLUA_EXPORT_FUNCTIONS(Test, getInt, getString, acceptString, getPtr)
//OOLUA_EXPORT_FUNCTIONS_CONST(Test)
//
//OOLUA_PROXY(Lol)
//	OOLUA_MFUNC(getInt)
//OOLUA_PROXY_END
//OOLUA_EXPORT_FUNCTIONS(Lol, getInt)
//OOLUA_EXPORT_FUNCTIONS_CONST(Lol)

void LuaScripter::registerScriptApi()
{
//	script->register_class<Lol>();
//	script->register_class<Test>();

	using namespace chaiscript;

	ChaiScript engine(Std_Lib::library());

	engine.add(user_type<Lol>(), "Lol");
	engine.add(constructor<Lol()>(), "Lol");
	engine.add(fun(&Lol::getInt), "getInt");

	engine.add(user_type<Test>(), "Test");
	engine.add(constructor<Test()>(), "Test");
	engine.add(fun(&Test::getInt), "getInt");
	engine.add(fun(&Test::getString), "getString");
	engine.add(fun(&Test::acceptString), "acceptString");
	engine.add(fun(&Test::getPtr), "getPtr");

	engine.eval(R"s(
		var t = Test();
		print(t.getInt());
		print(t.getString());
		print(t.acceptString("Lalala"));
		var l = t.getPtr();
		print(l.getInt());
	)s");

//	using namespace luabridge;

//	getGlobalNamespace(lua)
//		.beginNamespace("solo")
//			.beginClass<Texture>("Texture")
//			.endClass()
//
//			.beginClass<ResourceManager>("ResourceManager")
//			.endClass()
//
//			.beginClass<Device>("Device")
//				.addFunction("setWindowTitle", &Device::setWindowTitle)
//				.addFunction("getWindowTitle", &Device::getWindowTitle)
//			.endClass()
//
//			.beginClass<Node_ScriptWrap<Node>>("Node_ScriptWrap")
//				.addFunction("addScript", &Node_ScriptWrap<Node>::addScript)
//				.addFunction("removeScript", &Node_ScriptWrap<Node>::removeScript)
//			.endClass()
//			.deriveClass<Node, Node_ScriptWrap<Node>>("Node")
//				.addFunction("getId", &Node::getId)
//			.endClass()
//
//			.beginClass<Scene>("Scene")
//				.addFunction("createNode", &Scene::createNode)
//			.endClass()
//
//			.beginClass<Engine>("Engine")
//				.addFunction("getDevice", &Engine::getDevice)
//				.addFunction("getScene", &Engine::getScene)
//				.addFunction("getResourceManager", &Engine::getResourceManager)
//			.endClass()
//
//			.addVariable("engine", Engine::get(), false)
//		.endNamespace();
}