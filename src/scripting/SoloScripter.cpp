#include "scripting/SoloScripter.h"
#include "platform/SoloScripter_Chai.h"

using namespace solo;


shared<Scripter> ScripterFactory::create()
{
	return NEW2(Scripter_Chai);
}