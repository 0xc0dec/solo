#include "SoloLuaScripter.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloNode.h"
#include "SoloResourceManager.h"
#include "SoloTexture.h"
#include <chaiscript.hpp>
#include <chaiscript_stdlib.hpp>

using namespace solo;


LuaScripter::LuaScripter()
{
	registerScriptApi();
}


void LuaScripter::execString(const std::string& script)
{
}


void LuaScripter::execFile(const std::string& scriptFileName)
{
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

	int acceptPtr(std::shared_ptr<Lol> ptr)
	{
		return ptr->getInt();
	}
};


void LuaScripter::registerScriptApi()
{
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
	engine.add(fun(&Test::acceptPtr), "acceptPtr");

	engine.eval(R"s(
		var t = Test();
		print(t.getInt());
		print(t.getString());
		print(t.acceptString("Lalala"));
		var l = t.getPtr();
		print(l.getInt());
		print(t.acceptPtr(l));
	)s");
}