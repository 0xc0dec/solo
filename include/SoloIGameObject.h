#ifndef __SOLO_I_GAME_OBJECT_H__
#define __SOLO_I_GAME_OBJECT_H__

#include "SoloCommons.h"

namespace solo
{
	class IComponent;
	
	class IGameObject
	{
	public:
		virtual ~IGameObject() {}
		
		virtual str getName() = 0;
	};
}

#endif
