#pragma once

#include "SoloBase.h"

namespace solo
{
	class EngineCallback;
	class VideoDriver;
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
		ptr<VideoDriver> getVideoDriver() const;

		float getTimeDelta() const;

	private:
		Engine();
		~Engine();

		EngineCallback *_callback;
		ptr<Scene> _scene;
		ptr<Device> _device;
		ptr<VideoDriver> _driver;

		unsigned long _lastUpdateTime;
		float _timeDelta;

		inline void updateTime();
		void doRun(const EngineCreationArgs &args);
	};
}
