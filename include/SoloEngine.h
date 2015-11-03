#pragma once

#include "SoloBase.h"
#include "SoloEngineCreationArgs.h"

namespace solo
{
	class Scene;
	class Device;
	class FileSystem;
	class ResourceManager;

	class Engine
	{
	public:
		~Engine() {}

		static shared<Engine> create(const EngineCreationArgs &args);

		void run();
		void setStartCallback(std::function<void()> callback);
		void setShutdownCallback(std::function<void()> callback);
		void setShutdownRequestedCallback(std::function<bool()> callback);

		void requestShutdown();
		bool shutdownRequested() const;

		EngineMode getMode() const;

		Scene *getScene() const;
		Device *getDevice() const;
		FileSystem *getFileSystem() const;
		ResourceManager *getResourceManager() const;

	private:
		friend class EngineFactory;

		explicit Engine(const EngineCreationArgs &args);

		bool shutdown = false;
		EngineCreationArgs creationArgs;
		std::function<void()> startCallback{ [] {} };
		std::function<void()> shutdownCallback{ [] {} };
		std::function<bool()> shutdownRequestedCallback{ [] { return true; } };
		shared<Scene> scene;
		shared<Device> device;
		shared<FileSystem> fs;
		shared<ResourceManager> resourceManager;
	};

	inline void Engine::requestShutdown()
	{
		shutdown = true;
	}

	inline bool Engine::shutdownRequested() const
	{
		return shutdown;
	}

	inline EngineMode Engine::getMode() const
	{
		return creationArgs.mode;
	}

	inline Scene *Engine::getScene() const
	{
		return scene.get();
	}

	inline Device *Engine::getDevice() const
	{
		return device.get();
	}

	inline FileSystem *Engine::getFileSystem() const
	{
		return fs.get();
	}

	inline ResourceManager *Engine::getResourceManager() const
	{
		return resourceManager.get();
	}
}
