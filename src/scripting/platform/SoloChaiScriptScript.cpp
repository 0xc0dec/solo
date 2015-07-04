#include "SoloChaiScriptScript.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloChaiScriptScripter.h"

using namespace solo;


shared<ChaiScriptScript> ScriptFactory_Chai::create(Node* node, chaiscript::Boxed_Value obj)
{
	return NEW2(ChaiScriptScript, node, obj);
}


ChaiScriptScript::ChaiScriptScript(Node* node, chaiscript::Boxed_Value obj) :
	ComponentBase(node),
	obj(obj)
{
	auto engine = Engine::get();
	device = engine->getDevice();
	auto scripter = static_cast<ChaiScriptScripter*>(engine->getScripter());
	updateFunc = scripter->getEngine()->eval<std::function<void(chaiscript::Boxed_Value&, float)>>("update");
}


void ChaiScriptScript::update()
{
	updateFunc(obj, device->getTimeDelta());
}
