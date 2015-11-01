#pragma once

#include "SoloTypeId.h"
#include "SoloNode.h"

namespace solo
{
	class Scene;

	class Component
	{
	public:
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
		virtual ~Component() {}

		virtual size_t getTypeId() = 0;

		virtual void init() {}
		virtual void update() {}
		virtual void terminate() {}

	protected:
		Component() {}
	};

	template <class T>
	class ComponentBase: public Component
	{
	public:
		explicit ComponentBase(const Node& node): node(node) {}

		static size_t getId();
		virtual size_t getTypeId() override;
		Node getNode() const;

	protected:
		Node node;
	};

	template <class T>
	size_t ComponentBase<T>::getId()
	{
		return TypeId::get<T>();
	}

	template <class T>
	size_t ComponentBase<T>::getTypeId()
	{
		return getId();
	}

	template <class T>
	Node ComponentBase<T>::getNode() const
	{
		return node;
	}
}
