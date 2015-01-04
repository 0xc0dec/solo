#ifndef __SOLO_I_SCENE_H__
#define __SOLO_I_SCENE_H__

#include "SoloBase.h"

namespace solo
{
	class IComponent;
	class ISystem;

	class IScene
	{
	public:
		virtual ~IScene() {}

		virtual size_t createNode() = 0;
		virtual size_t createNodeWithTransform() = 0;

		virtual bool nodeExists(size_t node) = 0;

		virtual void addComponent(size_t node, ptr<IComponent> cmp) = 0;
		virtual ptr<IComponent> findComponent(size_t node, size_t typeId) = 0;
		virtual ptr<IComponent> getComponent(size_t node, size_t typeId) = 0;

		template <typename TComponent> ptr<TComponent> addComponent(size_t node)
		{
			auto cmp = NEW<TComponent>();
			auto base = PTR_SCAST<IComponent>(cmp);
			addComponent(node, base);
			return cmp;
		}

		template <typename TComponent> ptr<TComponent> getComponent(size_t node)
		{
			auto typeId = TComponent::getComponentTypeId();
			auto cmp = getComponent(node, typeId);
			return PTR_DCAST<TComponent>(cmp);
		}

		virtual void addSystem(ptr<ISystem> system, size_t targetComponentTypeId) = 0;
		virtual void removeSystem(ptr<ISystem> system) = 0;
		virtual bool systemAlreadyAdded(ptr<ISystem> system) = 0;

		template <typename TSystem> ptr<TSystem> addSystem(size_t targetComponentTypeId)
		{
			auto system = NEW<TSystem>();
			auto base = PTR_SCAST<ISystem>(system);
			addSystem(base, targetComponentTypeId);
			return system;
		}
	};
}

#endif