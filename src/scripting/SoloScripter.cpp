#include "scripting/SoloScripter.h"
#include "platform/SoloLuaScripter.h"

using namespace solo;


shared<Scripter> ScripterFactory::create()
{
	return NEW2(LuaScripter);
}