#include "SoloScene.h"
#include "SoloSceneNode.h"

using namespace solo;


Scene::Scene()
{
}


ptr<ISceneNode> Scene::createNode(const std::string &name)
{
	auto node = std::make_shared<SceneNode>(name);
	_nodes.push_back(node);
	return node;
}


void Scene::update()
{
	for (auto node: _nodes)
		node->update();
}
