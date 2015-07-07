#pragma once

#include "SoloTypeId.h"

namespace solo
{
	class RenderContext;
	class Node;

	class Component
	{
	public:
		virtual ~Component() {}

		virtual size_t getTypeId() = 0;

		// Called right before rendering. Use this to update object position, animation or any other game logic.
		virtual void update() {}

		// Called when the component is allowed to render anything.
		virtual void render(RenderContext& context) {}

		virtual void postRender() {}
	};

	template <class T>
	class ComponentBase: public Component
	{
	public:
		explicit ComponentBase(Node* node):
			node{node}
		{
		}

		static size_t getId()
		{
			return TypeId::get<T>();
		}

		virtual size_t getTypeId() override
		{
			return getId();
		}

		Node *getNode() const
		{
			return node;
		}

	protected:
		Node* node;

	private:
		ComponentBase(const ComponentBase& other) = delete;
		ComponentBase(ComponentBase&& other) = delete;
		ComponentBase& operator=(const ComponentBase& other) = delete;
		ComponentBase& operator=(ComponentBase&& other) = delete;
	};
}
