#ifndef __SOLO_I_SCENE_H__
#define __SOLO_I_SCENE_H__

#include "SoloCommons.h"
#include "SoloISceneNode.h"

namespace solo
{
	class ISceneNode;

	class IScene
	{
	public:
		virtual ~IScene() {}

		virtual sptr<ISceneNode> createNode(const str &name) = 0;
	};
}

#endif