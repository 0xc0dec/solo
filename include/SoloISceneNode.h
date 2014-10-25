#ifndef __SOLO_I_SCENE_NODE_H__
#define __SOLO_I_SCENE_NODE_H__

#include "SoloCommons.h"

namespace solo
{
	class IComponent;
	
	class ISceneNode
	{
	public:
		virtual ~ISceneNode() {}
		
		virtual str name() const = 0;
	};
}

#endif
