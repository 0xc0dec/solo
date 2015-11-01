#pragma once

#include "SoloBase.h"

namespace solo
{
	class Component;
	class Node;
	class Camera;
	class Renderer;
	class Engine;

	class Scene
	{
	public:
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
		~Scene();

		Engine *getEngine();

		shared<Node> createNode();

		void addComponent(size_t nodeId, shared<Component> cmp);
		void addComponent(size_t nodeId, shared<Component> cmp, size_t typeId);

		void removeComponent(size_t nodeId, size_t typeId);
		void removeAllComponents(size_t nodeId);
		void clear();

		Component* getComponent(size_t nodeId, size_t typeId) const;
		Component* findComponent(size_t nodeId, size_t typeId) const;

		void update();
		void render();

	private:
		friend class SceneFactory;
		
		using ComponentIterationWorker = std::function<void(size_t, Component*)>;
		using Components = std::unordered_map<size_t, std::unordered_map<size_t, shared<Component>>>;

		explicit Scene(Engine *engine);

		void iterateComponents(ComponentIterationWorker work);
		void syncCameraCache();

		Engine *engine;
		size_t nodeCounter = 0;
		bool cameraCacheDirty = true;

		std::vector<Camera*> cameraCache;
		Components components;
	};

	class SceneFactory
	{
		friend class Engine;
		static shared<Scene> create(Engine *engine);
	};
}
