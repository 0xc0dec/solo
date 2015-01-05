#pragma once

#include "SoloBase.h"
#include "SoloTypeId.h"

namespace solo
{
	class IComponent
	{
	public:
		virtual ~IComponent() {}

		virtual size_t getComponentTypeId() = 0;

		virtual void update() {}
	};

	template <class T>
	class Component: public IComponent
	{
	public:
		static size_t getId()
		{
			return solo::getTypeId<T>();
		}

		virtual size_t getComponentTypeId() override
		{
			return getId();
		}
	};
}
