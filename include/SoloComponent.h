#pragma once

#include "SoloTypeId.h"
#include "SoloNode.h"

namespace solo
{
	struct RenderContext;
	class Scene;

	class Component
	{
	public:
		virtual ~Component() {}
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual size_t getTypeId() = 0;

		virtual void init() {}
		virtual void update() {}
		virtual void render(RenderContext& context) {}
		virtual void postRender() {}
		virtual void terminate() {}

	protected:
		Component() {}
	};

	template <class T>
	class ComponentBase: public Component
	{
	public:
		explicit ComponentBase(Node node): node(node)
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

		Node getNode() const
		{
			return node;
		}

	protected:
		Node node;
	};
}
