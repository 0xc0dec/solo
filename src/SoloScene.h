#ifndef __SOLO_SCENE_H__
#define __SOLO_SCENE_H__

#include "SoloIScene.h"

namespace solo
{
	class SceneNode;

	class Scene: public IScene
	{
	public:
		virtual sptr<ISceneNode> createNode(const str &name);

		void update();

	private:
		list<sptr<SceneNode>> _nodes;
	};
}

#endif