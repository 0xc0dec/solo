#include "SoloScene.h"
#include "SoloSceneNode.h"

using namespace solo;

sptr<ISceneNode> Scene::createNode(const str name)
{
	return std::make_shared<SceneNode>(name);
}
