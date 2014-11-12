#ifndef __SOLO_I_SCENE_H__
#define __SOLO_I_SCENE_H__

#include "SoloCommon.h"
#include "SoloISceneNode.h"

namespace solo
{
	class ISceneNode;

	class IScene
	{
	public:
		virtual ~IScene() {}

		virtual ptr<ISceneNode> createNode(const String &name) = 0;
	};
}

#endif