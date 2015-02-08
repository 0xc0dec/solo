#pragma once

#include "SoloBase.h"

namespace solo
{
	class EngineCallback;
	class Scene;
	class Device;

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

		ptr<Scene> getScene() const;
		ptr<Device> getDevice() const;

		float getTimeDelta() const;

	private:
		Engine();
		~Engine();

		EngineCallback *callback;
		ptr<Scene> scene;
		ptr<Device> device;

		unsigned long lastUpdateTime;
		float timeDelta;

		inline void updateTime();
	};
}
