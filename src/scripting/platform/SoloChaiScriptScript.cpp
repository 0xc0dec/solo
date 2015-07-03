#include "SoloChaiScriptScript.h"
#include "SoloEngine.h"
#include "SoloDevice.h"

using namespace solo;


ChaiScriptScript::ChaiScriptScript(Node* node): Script(node)
{
	auto engine = Engine::get();
	device = engine->getDevice();
}


void ChaiScriptScript::update()
{
	if (updateCallback)
		updateCallback(device->getTimeDelta());
}


void ChaiScriptScript::setUpdateCallback(const std::function<void(float)>& callback)
{
	updateCallback = callback;
}
