#include "scripting/SoloScripter.h"
#include "platform/SoloChaiScriptScripter.h"

using namespace solo;


shared<Scripter> ScripterFactory::create()
{
	return NEW2(ChaiScriptScripter);
}