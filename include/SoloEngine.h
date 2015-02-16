#pragma once

#include "SoloBase.h"

namespace solo
{
	class EngineCallback;
	class Scene;
	class Device;
	class ResourceManager;

	struct EngineCreationArgs
	{
		int canvasWidth;
		int canvasHeight;
		int bits;
		int depth;
		bool fullScreen;
		const char *windowTitle;
	};

	class Engine
	{
	public:
		static Engine *get()
		{
			static Engine instance;
			return &instance;
		}

		void run(const EngineCreationArgs &launchArgs);
		void setCallback(EngineCallback* callback);

		Scene* getScene() const;
		Device* getDevice() const;
		ResourceManager* getResourceManager() const;

		float getTimeDelta() const;

	private:
		Engine();
		~Engine();

		EngineCallback *callback;
		shared<Scene> scene;
		shared<Device> device;
		shared<ResourceManager> resourceManager;

		float lastUpdateTime;
		float timeDelta;

		inline void updateTime();
	};
}
