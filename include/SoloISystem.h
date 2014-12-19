#ifndef __SOLO_I_SYSTEM_H__
#define __SOLO_I_SYSTEM_H__

#include "SoloBase.h"

namespace solo
{
	class IComponent;

	class ISystem
	{
	public:
		virtual ~ISystem() {}

		virtual void update(size_t node, ptr<IComponent> component) = 0;
	};
}

#endif