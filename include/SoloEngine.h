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

		static Engine *get();

		void run(const EngineCreationArgs &creationArgs);
		void setCallback(EngineCallback *callback);

		EngineMode getMode() const;

		Scene *getScene() const;
		Device *getDevice() const;
		FileSystem *getFileSystem() const;
		ResourceManager *getResourceManager() const;

	private:
		Engine();

		EngineCallback *callback = nullptr;
		shared<Scene> scene;
		shared<Device> device;
		shared<FileSystem> fs;
		shared<ResourceManager> resourceManager;
	};
}
