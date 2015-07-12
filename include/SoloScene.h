#pragma once

#include "SoloBase.h"
#include "SoloNode.h"

namespace solo
{
	class ModelRenderer;
	class Transform;
	class Component;
	class Node;
	class Camera;
	class RenderContext;

	class Scene
	{
	public:
		virtual ~Scene() {}

		shared<Node> createNode();

		void clear();

		void addComponent(size_t nodeId, shared<Component> cmp);
		void addComponent(size_t nodeId, shared<Component> cmp, size_t typeId);
		template <typename T, typename... Args> T* addComponent(size_t nodeId, Args... args)
		{
			auto cmp = NEW<T>(Node(this, nodeId), args...);
			auto base = STATIC_CAST<Component>(cmp);
			addComponent(nodeId, base);
			return cmp.get();
		}

		void removeComponent(size_t nodeId, size_t typeId);
		void removeAllComponents(size_t nodeId);
		template <typename T> void removeComponent(size_t nodeId)
		{
			removeComponent(nodeId, T::getId());
		}

		Component* getComponent(size_t nodeId, size_t typeId);
		template <typename T> T* getComponent(size_t nodeId)
		{
			auto typeId = T::getId();
			auto cmp = getComponent(nodeId, typeId);
			return static_cast<T*>(cmp);
		}

		Component* findComponent(size_t nodeId, size_t typeId);
		template <typename T> T* findComponent(size_t nodeId)
		{
			auto typeId = T::getId();
			auto cmp = findComponent(nodeId, typeId);
			return static_cast<T*>(cmp);
		}

		void update();
		void render();

	private:
		friend class SceneFactory;

		Scene() {}
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		typedef std::function<void(size_t, shared<Component>)> ComponentInterationWorker;
		void iterateComponents(ComponentInterationWorker work);
		std::vector<shared<Camera>> getCameras();

		size_t nodeCounter = 0;
		std::map<size_t, std::map<size_t, shared<Component>>> components;
	};

	template<> Transform* Scene::addComponent(size_t nodeId);
	template<> Camera* Scene::addComponent(size_t nodeId);
	template<> ModelRenderer* Scene::addComponent(size_t nodeId);

	class SceneFactory
	{
		friend class Engine;
		static shared<Scene> create();
	};
}
