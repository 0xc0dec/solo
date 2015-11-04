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
		friend class SceneFactory;
		
		using ComponentIterationWorker = std::function<void(size_t, Component*)>;
		using Components = std::unordered_map<size_t, std::unordered_map<size_t, shared<Component>>>;

		explicit Scene(Device *device);

		void iterateComponents(ComponentIterationWorker work);
		void syncCameraCache();

		Device *device;
		size_t nodeCounter = 0;
		bool cameraCacheDirty = true;

		std::vector<Camera*> cameraCache;
		Components components;
	};

	inline Device* Scene::getDevice() const
	{
		return device;
	}

	class SceneFactory
	{
		friend class Device;
		static shared<Scene> create(Device *device);
	};
}
