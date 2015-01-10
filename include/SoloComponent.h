#pragma once

#include "SoloBase.h"
#include "SoloTypeId.h"

namespace solo
{
	struct RenderContext;

	class Component
	{
	public:
		virtual ~Component() {}

		virtual size_t getTypeId() = 0;

		// Called right before rendering. Use this to update object position, animation or any other game logic.
		virtual void update() {}

		// Called when the component is allowed to render anything.
		virtual void render(const RenderContext& context) {}
	};

	template <class T>
	class ComponentBase: public Component
	{
	public:
		explicit ComponentBase(size_t node):
			_node(node)
		{
		}

		static size_t getId()
		{
			return solo::getTypeId<T>();
		}

		virtual size_t getTypeId() override
		{
			return getId();
		}

		size_t getNode() const
		{
			return _node;
		}

	protected:
		size_t _node;
	};
}
