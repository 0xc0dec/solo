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

		void addComponent(Node* node, ptr<Component> cmp);
		void removeComponent(Node* node, size_t typeId);
		ptr<Component> getComponent(Node* node, size_t typeId);
		ptr<Component> findComponent(Node* node, size_t typeId);

		void update();
		void render();

	private:
		friend class SceneFactory;

		Scene();

		typedef std::map<size_t, std::map<size_t, ptr<Component>>> Components;
		typedef std::map<size_t, ptr<Node>> Nodes;

		int nodeCounter;
		Components components;
		Nodes nodes;

		void ensureNodeExists(size_t nodeId);
		std::vector<ptr<Camera>> getCameras();
	};

	class SceneFactory
	{
		friend class Engine;
		static ptr<Scene> create();
	};
}
