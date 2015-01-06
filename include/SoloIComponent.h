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

		// Called right before rendering. Use this to update object position, animation or any other game logic.
		virtual void update() {}

		// Called when the component is allowed to render anything.
		virtual void render() {}
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
