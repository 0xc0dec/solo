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

		template<> Camera* addComponent<Camera>()
		{
			auto camera = Camera::create(this);
			scene->addComponent(this, camera);
			return camera.get();
		}

		template<> Transform* addComponent<Transform>()
		{
			auto transform = Transform::create(this);
			scene->addComponent(this, transform);
			return transform.get();
		}

		template<> ModelRenderer* addComponent<ModelRenderer>()
		{
			auto renderer = ModelRenderer::create(this);
			scene->addComponent(this, renderer);
			return renderer.get();
		}

		template <typename T> void removeComponent()
		{
			scene->removeComponent(this, T::getId());
		}

		template <typename T> T* getComponent()
		{
			auto typeId = T::getId();
			auto cmp = scene->getComponent(this, typeId);
			return static_cast<T*>(cmp.get());
		}

		template <typename T> T* findComponent()
		{
			auto typeId = T::getId();
			auto cmp = scene->findComponent(this, typeId);
			return static_cast<T*>(cmp.get());
		}

	private:
		friend class NodeFactory;

		Scene* scene;

		Node(Scene* scene) : scene(scene)
		{
		}

		Node(const Node& other);
		Node(Node&& other);
		Node& operator=(const Node& other);
		Node& operator=(Node&& other);
	};

	class NodeFactory
	{
		friend class Scene;
		static ptr<Node> createNode(Scene *scene);
	};
}