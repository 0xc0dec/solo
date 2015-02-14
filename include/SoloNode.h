#pragma once

#include "SoloScene.h"
#include "SoloModelRenderer.h"
#include "SoloCamera.h"
#include "SoloTransform.h"

namespace solo
{
	class Node
	{
	public:
		~Node() {}

		size_t getId() const
		{
			return reinterpret_cast<size_t>(this);
		}

		template <typename T> T* addComponent()
		{
			auto cmp = NEW<T>(this);
			auto base = CAST_PTR_STATIC<Component>(cmp);
			scene->addComponent(this, base);
			return cmp.get();
		}

		template <typename T> void removeComponent()
		{
			scene->removeComponent(this, T::getId());
		}

		template <typename T> T* getComponent()
		{
			auto typeId = T::getId();
			auto cmp = scene->getComponent(this, typeId);
			return static_cast<T*>(cmp);
		}

		template <typename T> T* findComponent()
		{
			auto typeId = T::getId();
			auto cmp = scene->findComponent(this, typeId);
			return static_cast<T*>(cmp);
		}

	private:
		friend class NodeFactory;
	
		Node(Scene* scene) : scene(scene) {}
		Node(const Node& other) = delete;
		Node(Node&& other) = delete;
		Node& operator=(const Node& other) = delete;
		Node& operator=(Node&& other) = delete;

		Scene* scene;
	};

	class NodeFactory
	{
		friend class Scene;
		static shared<Node> createNode(Scene *scene);
	};

	template<> Transform* Node::addComponent<Transform>();
	template<> Camera* Node::addComponent<Camera>();
	template<> ModelRenderer* Node::addComponent<ModelRenderer>();
}