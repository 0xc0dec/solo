#pragma once

#include "SoloBase.h"

namespace solo
{
	class Camera;
	class ModelRenderer;
	class Component;

	class Scene
	{
	public:
		virtual ~Scene() {}

		static ptr<Scene> create();

		size_t createEmptyNode();
		size_t createNode();

		bool nodeExists(size_t node);

		ptr<Camera> addCamera(size_t node);
		ptr<ModelRenderer> addModelRenderer(size_t node);

		void addComponent(size_t node, ptr<Component> cmp);
		template <typename TComponent> ptr<TComponent> addComponent(size_t node)
		{
			auto cmp = NEW<TComponent>();
			auto base = CAST_PTR_STATIC<Component>(cmp);
			addComponent(node, base);
			return cmp;
		}

		ptr<Component> findComponent(size_t node, size_t typeId);
		template <typename TComponent> ptr<TComponent> findComponent(size_t node)
		{
			auto typeId = TComponent::getComponentTypeId();
			auto cmp = findComponent(node, typeId);
			return CAST_PTR_STATIC<TComponent>(cmp);
		}

		ptr<Component> getComponent(size_t node, size_t typeId);
		template <typename TComponent> ptr<TComponent> getComponent(size_t node)
		{
			auto typeId = TComponent::getId();
			auto cmp = getComponent(node, typeId);
			return CAST_PTR_STATIC<TComponent>(cmp);
		}

		void update();
		void render();

	private:
		Scene();

		typedef std::map<size_t, std::map<size_t, ptr<Component>>> NodeComponents;

		int _nodeCounter;
		ptr<Camera> _primaryCamera;
		NodeComponents _nodeComponents;

		void ensureNodeExists(size_t node);
	};
}
