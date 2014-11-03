#ifndef __SOLO_SCENE_H__
#define __SOLO_SCENE_H__

#include <list>
#include "SoloIScene.h"

namespace solo
{
	class SceneNode;

	class Scene: public IScene
	{
	public:
		virtual sptr<ISceneNode> createNode(const str &name) override;
		
		void update();

	private:
		std::list<sptr<SceneNode>> _nodes;
	};
}

#endif