#pragma once

#include "SoloScene.h"
#include "SoloModelRenderer.h"
#include "SoloCamera.h"
#include "SoloTransform.h"
#include "scripting/SoloScriptedNode.h"

namespace solo
{
	class Node: public ScriptedNode<Node>
	{
	public:
		~Node() {}

		long getId() const;

		template <typename T, typename... Args>
		T* addComponent(Args... args)
		{
			auto cmp = NEW<T>(this, args...);
			auto base = STATIC_CAST<Component>(cmp);
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

		Scene* scene = nullptr;
	};

	class NodeFactory
	{
		friend class Scene;
		static shared<Node> createNode(Scene *scene);
	};

	template<> Transform* Node::addComponent();
	template<> Camera* Node::addComponent();
	template<> ModelRenderer* Node::addComponent();
	template<> Script* Node::addComponent(const std::string& callbackObjectName);
}