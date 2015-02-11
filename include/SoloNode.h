#pragma once

#include "SoloScene.h"
#include "SoloModelRenderer.h"
#include "SoloCamera.h"
#include "SoloTransform.h"

namespace solo
{
	class Node
	{
		friend class NodeFactory;

	public:
		~Node() {}

		size_t getId() const
		{
			return reinterpret_cast<size_t>(this);
		}

		template <typename T> ptr<T> addComponent()
		{
			auto cmp = NEW<T>(this);
			auto base = CAST_PTR_STATIC<Component>(cmp);
			scene->addComponent(getId(), base);
			return cmp;
		}

		template<> ptr<Camera> addComponent<Camera>()
		{
			auto camera = Camera::create(this);
			scene->addComponent(getId(), camera);
			return camera;
		}

		template<> ptr<Transform> addComponent<Transform>()
		{
			auto transform = Transform::create(this);
			scene->addComponent(getId(), transform);
			return transform;
		}

		template<> ptr<ModelRenderer> addComponent<ModelRenderer>()
		{
			auto renderer = ModelRenderer::create(this);
			scene->addComponent(getId(), renderer);
			return renderer;
		}

		template <typename T> void removeComponent()
		{
			scene->removeComponent(getId(), T::getId());
		}

		template <typename T> ptr<T> getComponent()
		{
			auto typeId = T::getId();
			auto cmp = scene->getComponent(getId(), typeId);
			return CAST_PTR_STATIC<T>(cmp);
		}

		template <typename T> ptr<T> findComponent()
		{
			auto typeId = T::getId();
			auto cmp = scene->findComponent(getId(), typeId);
			return CAST_PTR_STATIC<T>(cmp);
		}

	private:
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

		static ptr<Node> createNode(Scene *scene)
		{
			return NEW2(Node, scene);
		}
	};
}