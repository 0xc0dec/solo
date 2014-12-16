#ifndef __SOLO_I_SCENE_H__
#define __SOLO_I_SCENE_H__

#include "SoloBase.h"
#include "SoloISceneNode.h"

namespace solo
{
	class ISceneNode;

	class IScene
	{
	public:
		virtual ~IScene() {}

		virtual ptr<ISceneNode> createNode(const std::string &name) = 0;
	};
}

#endif