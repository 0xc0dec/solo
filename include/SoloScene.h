#pragma once

#include "SoloBase.h"

namespace solo
{
	class Component;
	class Node;
	class Camera;

	class Scene
	{
	public:
		virtual ~Scene() {}

		Node* createEmptyNode();
		Node* createNode();

		void addComponent(Node* node, shared<Component> cmp);
		void removeComponent(Node* node, size_t typeId);
		shared<Component> getComponent(Node* node, size_t typeId);
		shared<Component> findComponent(Node* node, size_t typeId);

		void update();
		void render();

	private:
		friend class SceneFactory;

		Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		typedef std::map<size_t, std::map<size_t, shared<Component>>> Components;
		typedef std::map<size_t, shared<Node>> Nodes;

		int nodeCounter;
		Components components;
		Nodes nodes;

		void ensureNodeExists(size_t nodeId);
		std::vector<shared<Camera>> getCameras();
	};

	class SceneFactory
	{
		friend class Engine;
		static shared<Scene> create();
	};
}
