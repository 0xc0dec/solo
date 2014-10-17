#ifndef __SOLO__GAME_OBJECT_H__
#define	__SOLO__GAME_OBJECT_H__

#include "SoloIGameObject.h"

namespace solo
{
	class GameObject: public IGameObject
	{
	public:
		GameObject(const str &name);
		
		virtual str getName() override { return _name; }

	protected:
		str _name;
	};
}

#endif
