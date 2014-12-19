#ifndef __SOLO_I_COMPONENT_H__
#define __SOLO_I_COMPONENT_H__

#include "SoloBase.h"

namespace solo
{
	class IComponent
	{
	public:
		virtual ~IComponent() {}

		virtual size_t getTypeId() = 0;
	};
}

#endif
