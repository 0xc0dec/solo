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
		~Engine();

		static shared<Engine> create(const EngineCreationArgs &args);

		void run();
		void setCallback(EngineCallback *callback);

		Scene *getScene() const;
		Device *getDevice() const;
		FileSystem *getFileSystem() const;
		ResourceManager *getResourceManager() const;

	private:
		friend class EngineFactory;

		explicit Engine(const EngineCreationArgs &args);

		EngineCreationArgs creationArgs;
		EngineCallback *callback = nullptr;
		shared<Scene> scene;
		shared<Device> device;
		shared<FileSystem> fs;
		shared<ResourceManager> resourceManager;
	};
}
