#include "SoloNode.h"
#include "SoloScene.h"

using namespace solo;


Node::Node(Scene* scene, size_t nodeId) :
	scene(scene),
	id(nodeId)
{
}


size_t Node::getId() const
{
	return id;// reinterpret_cast<size_t>(this);
}


Scene* Node::getScene() const
{
	return scene;
}


void Node::removeAllComponents()
{
	scene->removeAllComponents(this->getId());
}
