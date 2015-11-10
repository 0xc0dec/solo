#pragma once

#include "SoloBase.h"

namespace solo
{
	class Component;
	class Node;
	class Camera;
	class Renderer;
	class Device;

	class Scene
	{
	public:
		static shared<Scene> create(Device *device);

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
		~Scene();

		Device *getDevice() const;

		shared<Node> createNode();

		void addComponent(size_t nodeId, shared<Component> cmp);
		void addComponentWithTypeId(size_t nodeId, shared<Component> cmp, size_t typeId);

		void removeComponent(size_t nodeId, size_t typeId);
		void removeAllComponents(size_t nodeId);
		void clear();

		Component* getComponent(size_t nodeId, size_t typeId) const;
		Component* findComponent(size_t nodeId, size_t typeId) const;

		void update();

		void render();
		void renderWithCamera(Camera *camera);

	private:
		using ComponentIterationWorker = std::function<void(size_t, Component*)>;
		using Components = std::unordered_map<size_t, std::unordered_map<size_t, shared<Component>>>;

		explicit Scene(Device *device);

		void iterateComponents(ComponentIterationWorker work);
		void updateCameraCache();
		void updateRenderQueue();

		Device *device;
		size_t nodeCounter = 0;
		bool cameraCacheDirty = true;

		size_t componentCount = 0;

		std::vector<Camera*> cameraCache;
		std::list<Component*> renderQueue;

		Components components;
	};

	inline Device* Scene::getDevice() const
	{
		return device;
	}
}
