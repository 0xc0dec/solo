#pragma once

#include "SoloBase.h"

namespace solo
{
	class Camera;
	class Component;

	class Scene
	{
	public:
		virtual ~Scene() {}

		static ptr<Scene> create();

		size_t createEmptyNode();
		size_t createNode();

		bool nodeExists(size_t node);

		template <typename T> ptr<T> addComponent(size_t node)
		{
			auto cmp = NEW<T>();
			auto base = CAST_PTR_STATIC<Component>(cmp);
			addComponent(node, base);
			return cmp;
		}

		template <typename T> void removeComponent(size_t node)
		{
			removeComponent(node, T::getId());
		}

		template <typename T> ptr<T> getComponent(size_t node)
		{
			auto typeId = T::getId();
			auto cmp = getComponent(node, typeId);
			return CAST_PTR_STATIC<T>(cmp);
		}

		template <typename T> ptr<T> findComponent(size_t node)
		{
			auto typeId = T::getTypeId();
			auto cmp = findComponent(node, typeId);
			return CAST_PTR_STATIC<T>(cmp);
		}

		void update();
		void render();

	private:
		Scene();

		typedef std::map<size_t, std::map<size_t, ptr<Component>>> Components;

		int _nodeCounter;
		Components _components;

		void ensureNodeExists(size_t node);

		void addComponent(size_t node, ptr<Component> cmp);
		void removeComponent(size_t node, size_t typeId);
		ptr<Component> getComponent(size_t node, size_t typeId);
		ptr<Component> findComponent(size_t node, size_t typeId);

		std::vector<ptr<Camera>> getCameras();
	};
}
