#pragma once

#include "SoloBase.h"

namespace solo
{
	class IComponent;
	class ISystem;
	class ICamera;

	class IScene
	{
	public:
		virtual ~IScene() {}

		// Creates node without components
		virtual size_t createEmptyNode() = 0;

		// Creates node with transform
		virtual size_t createNode() = 0;

		virtual bool nodeExists(size_t node) = 0;

		virtual ptr<ICamera> createCamera(size_t node) = 0;

		virtual void addComponent(size_t node, ptr<IComponent> cmp) = 0;
		virtual ptr<IComponent> findComponent(size_t node, size_t typeId) = 0;
		virtual ptr<IComponent> getComponent(size_t node, size_t typeId) = 0;

		template <typename TComponent> ptr<TComponent> addComponent(size_t node)
		{
			auto cmp = NEW<TComponent>();
			auto base = PTR_CAST_STATIC<IComponent>(cmp);
			addComponent(node, base);
			return cmp;
		}

		template <typename TComponent> ptr<TComponent> findComponent(size_t node)
		{
			auto typeId = TComponent::getComponentTypeId();
			auto cmp = findComponent(node, typeId);
			return PTR_CAST_DYNAMIC<TComponent>(cmp);
		}

		template <typename TComponent> ptr<TComponent> getComponent(size_t node)
		{
			auto typeId = TComponent::getId();
			auto cmp = getComponent(node, typeId);
			return PTR_CAST_DYNAMIC<TComponent>(cmp);
		}
	};
}
