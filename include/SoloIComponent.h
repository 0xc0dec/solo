#ifndef __SOLO_I_COMPONENT_H__
#define __SOLO_I_COMPONENT_H__

#include "SoloCommons.h"
#include "SoloISceneNode.h"

namespace solo
{
	class IComponent
	{
	public:
		virtual ~IComponent() {}

		virtual str id() = 0;
		virtual void update() {}
	};
}

#endif
