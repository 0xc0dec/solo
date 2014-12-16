#ifndef __SOLO_SCENE_H__
#define __SOLO_SCENE_H__

#include "SoloBaseInternal.h"
#include "SoloIScene.h"

namespace solo
{
	class SceneNode;

	class Scene: public IScene
	{
	public:
		explicit Scene();

		virtual ptr<ISceneNode> createNode(const std::string &name) override;
		
		void update();

	private:
		std::list<ptr<SceneNode>> _nodes;
	};
}

#endif