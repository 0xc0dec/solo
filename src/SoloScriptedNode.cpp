#include "SoloScriptedNode.h"
#include "SoloNode.h"

using namespace solo;


template<> void ScriptedNode<Node>::addScript(const std::string &callbackObjectName)
{
	static_cast<Node*>(this)->addComponent<Script, const std::string&>(callbackObjectName);
}