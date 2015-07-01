#include "SoloLuaScripter.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloNode.h"

#include <LuaBridge.h>

using namespace solo;

/*
template <class T>
class Base
{
public:
	int baseFunc()
	{
		return static_cast<T*>(this)->aFunc() * 10;
	}
};

class A : public Base<A>
{
public:
	int aFunc()
	{
		return 5;
	}
};
*/

template <class T>
struct A
{
	int aFunc()
	{
		return static_cast<T*>(this)->bFunc();
	}
};

struct B : public A<B>
{
	int bFunc()
	{
		return 5;
	}
};


LuaScripter::LuaScripter()
{
	lua = luaL_newstate();
	luaL_openlibs(lua);

	using namespace luabridge;

	getGlobalNamespace(lua)
		.beginNamespace("solo")
			.beginClass<Device>("Device")
				.addFunction("setWindowTitle", &Device::setWindowTitle)
				.addFunction("getWindowTitle", &Device::getWindowTitle)
			.endClass()

			.beginClass<Node>("Node")
				.addFunction("getId", &Node::getId)
			.endClass()

			.beginClass<Scene>("Scene")
				.addFunction("createNode", &Scene::createNode)
			.endClass()

			.beginClass<Engine>("Engine")
				.addFunction("getDevice", &Engine::getDevice)
				.addFunction("getScene", &Engine::getScene)
			.endClass()

			.beginClass <A<B>>("A")
				.addFunction("func", &A<B>::aFunc)
			.endClass()
			.deriveClass <B, A<B>>("B")
				.addConstructor<void(*)(void)>()
			.endClass()

			.addVariable("engine", Engine::get(), false)
		.endNamespace();

	/*
	.beginClass<A>("A")
	.addConstructor<void(*)(void)>()
	.addFunction("func", &A::baseFunc)
	.endClass()
	*/
}


void LuaScripter::execString(const std::string& script)
{
	luaL_dostring(lua, script.c_str());
}


void LuaScripter::execFile(const std::string& scriptFileName)
{
	luaL_dofile(lua, scriptFileName.c_str());
}
