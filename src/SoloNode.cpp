#include "SoloNode.h"

using namespace solo;


ptr<Node> NodeFactory::createNode(Scene* scene)
{
	return NEW2(Node, scene);
}
