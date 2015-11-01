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
		Node(Scene* scene, size_t nodeId);

		inline size_t getId() const;

		inline Scene* getScene() const;

		template <typename T, typename... Args>
		static inline T* addComponent(Scene *scene, size_t nodeId, Args... args);

		template <typename T, typename... Args>
		inline T* addComponent(Args... args);

		template <typename T>
		static inline T* getComponent(Scene *scene, size_t nodeId);

		template <typename T>
		inline T* getComponent();

		template <typename T>
		static inline T* findComponent(Scene *scene, size_t nodeId);

		template <typename T>
		inline T* findComponent() const;

		template <typename T>
		inline static void removeComponent(Scene *scene, size_t nodeId);

		template <typename T>
		inline void removeComponent();

		inline void removeAllComponents();

	private:
		Scene* scene;
		size_t id;
	};


	size_t Node::getId() const
	{
		return id;
	}

	Scene* Node::getScene() const
	{
		return scene;
	}

	template <typename T, typename... Args>
	T* Node::addComponent(Scene *scene, size_t nodeId, Args... args)
	{
		auto cmp = SL_NEW<T>(Node(scene, nodeId), args...);
		auto base = SL_STATIC_CAST<Component>(cmp);
		scene->addComponent(nodeId, base);
		return cmp.get();
	}

	template <typename T, typename... Args>
	T* Node::addComponent(Args... args)
	{
		return addComponent<T>(scene, id, args...);
	}

	template <typename T>
	T* Node::getComponent(Scene *scene, size_t nodeId)
	{
		auto typeId = T::getId();
		auto cmp = scene->getComponent(nodeId, typeId);
		return static_cast<T*>(cmp);
	}

	template <typename T>
	T* Node::getComponent()
	{
		return getComponent<T>(scene, id);
	}

	template <typename T>
	T* Node::findComponent(Scene *scene, size_t nodeId)
	{
		auto typeId = T::getId();
		auto cmp = scene->findComponent(nodeId, typeId);
		return static_cast<T*>(cmp);
	}

	template <typename T>
	inline T* Node::findComponent() const
	{
		return findComponent<T>(scene, id);
	}

	template <typename T>
	void Node::removeComponent(Scene *scene, size_t nodeId)
	{
		scene->removeComponent(nodeId, T::getId());
	}

	template <typename T>
	void Node::removeComponent()
	{
		removeComponent<T>(scene, id);
	}

	void Node::removeAllComponents()
	{
		scene->removeAllComponents(id);
	}

	template<> Transform* Node::addComponent();
	template<> Camera* Node::addComponent();
	template<> ModelRenderer* Node::addComponent();
}