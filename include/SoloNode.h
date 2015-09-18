#pragma once

#include "SoloScene.h"

namespace solo
{
	class Transform;
	class Camera;
	class ModelRenderer;

	// A convenient wrapper for working with components. There's no real "Node" in the engine
	class Node
	{
	public:
		Node(Scene* scene, size_t nodeId):
			scene(scene), id(nodeId)
		{
		}

		inline size_t getId() const;
		inline Scene* getScene() const;

		template <typename T, typename... Args>
		static T* addComponent(Scene *scene, size_t nodeId, Args... args)
		{
			auto cmp = NEW<T>(Node(scene, nodeId), args...);
			auto base = STATIC_CAST<Component>(cmp);
			scene->addComponent(nodeId, base);
			return cmp.get();
		}

		template <typename T, typename... Args>
		T* addComponent(Args... args)
		{
			return addComponent<T>(scene, id, args...);
		}

		template <typename T>
		static T* getComponent(Scene *scene, size_t nodeId)
		{
			auto typeId = T::getId();
			auto cmp = scene->getComponent(nodeId, typeId);
			return static_cast<T*>(cmp);
		}

		template <typename T>
		T* getComponent()
		{
			return getComponent<T>(scene, id);
		}

		template <typename T>
		static T* findComponent(Scene *scene, size_t nodeId)
		{
			auto typeId = T::getId();
			auto cmp = scene->findComponent(nodeId, typeId);
			return static_cast<T*>(cmp);
		}

		template <typename T>
		T* findComponent() const
		{
			return findComponent<T>(scene, id);
		}

		template <typename T>
		static void removeComponent(Scene *scene, size_t nodeId)
		{
			scene->removeComponent(nodeId, T::getId());
		}

		template <typename T>
		void removeComponent()
		{
			removeComponent<T>(scene, id);
		}

		inline void removeAllComponents();

	private:
		Scene* scene;
		size_t id;
	};

	inline size_t Node::getId() const
	{
		return id;
	}

	Scene* Node::getScene() const
	{
		return scene;
	}

	void Node::removeAllComponents()
	{
		scene->removeAllComponents(id);
	}

	template<> Transform* Node::addComponent();
	template<> Camera* Node::addComponent();
	template<> ModelRenderer* Node::addComponent();
}