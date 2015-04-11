#pragma once

#include "SoloBase.h"
#include "SoloEngineCreationArgs.h"

namespace solo
{
	class EngineCallback;
	class Scene;
	class Device;
	class FileSystem;
	class ResourceManager;

	class Engine
	{
	public:
		static Engine *get();

		void run(const EngineCreationArgs &creationArgs);
		void setCallback(EngineCallback* callback);

		Scene* getScene() const;
		Device* getDevice() const;
		FileSystem* getFileSystem() const;
		ResourceManager* getResourceManager() const;

		float getTimeDelta() const;

	private:
		Engine();
		~Engine();

		EngineCallback *callback = nullptr;
		shared<Scene> scene;
		shared<Device> device;
		shared<FileSystem> fs;
		shared<ResourceManager> resourceManager;

		float lastUpdateTime = 0;
		float timeDelta = 0;

		inline void updateTime();
	};
}
