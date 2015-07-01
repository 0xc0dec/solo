#include "scripting/SoloNode_ScriptWrap.h"
#include "SoloNode.h"

using namespace solo;


template<> void Node_ScriptWrap<Node>::addScript(const std::string &callbackObjectName)
{
	static_cast<Node*>(this)->addComponent<Script, const std::string&>(callbackObjectName);
}


template<> void Node_ScriptWrap<Node>::removeScript()
{
	static_cast<Node*>(this)->removeComponent<Script>();
}