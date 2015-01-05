#pragma once

#include "SoloBaseInternal.h"
#include "SoloIEngine.h"

namespace solo
{
	class IEngineCallback;
	class VideoDriver;
	class Scene;
	class Device;

	class Engine: public IEngine
	{
	public:
		static Engine *getEngine()
		{
			static Engine instance;
			return &instance;
		}

		void run(const EngineCreationArgs &launchArgs) override;
		void setCallback(IEngineCallback* callback) override;

		virtual ptr<IScene> getScene() const override;
		virtual ptr<IDevice> getDevice() const override;
		virtual ptr<IVideoDriver> getVideoDriver() const override;

		virtual float getTimeDelta() const override;

	private:
		Engine();
		virtual ~Engine();

		IEngineCallback *_callback;
		ptr<Scene> _scene;
		ptr<Device> _device;
		ptr<VideoDriver> _driver;

		unsigned long _lastUpdateTime;
		float _timeDelta;

		inline void updateTime();
		void doRun(const EngineCreationArgs &args);
	};
}
