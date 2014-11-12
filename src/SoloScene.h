#ifndef __SOLO_SCENE_H__
#define __SOLO_SCENE_H__

#include "SoloCommonInternal.h"
#include "SoloIScene.h"

namespace solo
{
	class SceneNode;

	class Scene: public IScene
	{
	public:
		explicit Scene();

		virtual ptr<ISceneNode> createNode(const String &name) override;
		
		void update();

	private:
		List<ptr<SceneNode>> _nodes;
	};
}

#endif