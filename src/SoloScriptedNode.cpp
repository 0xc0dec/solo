#include "SoloScriptedNode.h"
#include "SoloNode.h"

using namespace solo;


template<> void ScriptedNode<Node>::addScriptComponent(const std::string &componentObjectName)
{
	static_cast<Node*>(this)->addComponent<Script, const std::string&>(componentObjectName);
}