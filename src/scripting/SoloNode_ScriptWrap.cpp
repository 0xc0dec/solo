#include "scripting/SoloNode_ScriptWrap.h"
#include "SoloNode.h"

using namespace solo;


template<> Script* Node_ScriptWrap<Node>::addScript()
{
	return static_cast<Node*>(this)->addComponent<Script>();
}


template<> void Node_ScriptWrap<Node>::removeScript()
{
	static_cast<Node*>(this)->removeComponent<Script>();
}