#pragma once

#include "SoloBase.h"

namespace solo
{
	class Component;
	class Node;
	class Camera;
	class RenderContext;

	class Scene
	{
	public:
		virtual ~Scene() {}

		Node* createEmptyNode();
		Node* createNode();

		void addComponent(Node* node, shared<Component> cmp);
		void removeComponent(Node* node, size_t typeId);
		Component* getComponent(Node* node, size_t typeId);
		Component* findComponent(Node* node, size_t typeId);

		void update();
		void render();

	private:
		friend class SceneFactory;

		Scene() {}
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void iterateComponents(std::function<void(shared<Node>, shared<Component>)> work);

		int nodeCounter{ 0 };
		std::map<size_t, std::map<size_t, shared<Component>>> components;
		std::map<size_t, shared<Node>> nodes;

		void ensureNodeExists(size_t nodeId);
		std::vector<shared<Camera>> getCameras();
	};

	class SceneFactory
	{
		friend class Engine;
		static shared<Scene> create();
	};
}
