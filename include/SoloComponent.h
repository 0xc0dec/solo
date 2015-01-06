#pragma once

#include "SoloBase.h"
#include "SoloTypeId.h"

namespace solo
{
	class Component
	{
	public:
		virtual ~Component() {}

		virtual size_t getTypeId() = 0;

		// Called right before rendering. Use this to update object position, animation or any other game logic.
		virtual void update() {}

		// Called when the component is allowed to render anything.
		virtual void render() {}
	};

	template <class T>
	class ComponentBase: public Component
	{
	public:
		static size_t getId()
		{
			return solo::getTypeId<T>();
		}

		virtual size_t getTypeId() override
		{
			return getId();
		}
	};
}
