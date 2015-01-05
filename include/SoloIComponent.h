#pragma once

#include "SoloBase.h"
#include "SoloTypeId.h"

#define DECLARE_COMPONENT(ComponentType) \
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

		virtual void update() {}
	};
}
