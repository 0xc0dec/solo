#include "scripting/SoloScripter.h"
#include "platform/SoloScripter_Chai.h"
#include "platform/SoloScripter_Lua.h"

using namespace solo;


shared<Scripter> ScripterFactory::create()
{
//	return NEW2(Scripter_Chai);
	return NEW2(Scripter_Lua);
}