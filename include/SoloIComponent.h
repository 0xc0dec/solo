#ifndef __SOLO_I_COMPONENT_H__
#define __SOLO_I_COMPONENT_H__

#include "SoloBase.h"

namespace solo
{
	class ISceneNode;

	class IComponent
	{
	public:
		virtual ~IComponent() {}

		virtual std::string id() = 0;

		virtual void update() {}
		virtual void start() {}
	};
}

#endif
