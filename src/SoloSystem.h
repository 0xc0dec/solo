#ifndef __SOLO_SYSTEM_H__
#define __SOLO_SYSTEM_H__

#include "SoloISystem.h"

namespace solo
{
	class System: public ISystem
	{
	public:
		virtual void update(size_t node, ptr<IComponent> component) override;
	};
}

#endif