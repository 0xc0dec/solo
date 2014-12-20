#ifndef __SOLO_I_COMPONENT_H__
#define __SOLO_I_COMPONENT_H__

#include "SoloBase.h"
#include "SoloTypeId.h"

#define DECLARE_CMP_ID(ComponentType) \
	static size_t getComponentTypeId() \
	{ \
		return solo::getTypeId<ComponentType>(); \
	} \
	virtual size_t getTypeId() override \
	{ \
		return getComponentTypeId(); \
	}

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
