#ifndef __SOLO_ENGINE_H__
#define __SOLO_ENGINE_H__

#include "SoloBaseInternal.h"
#include "SoloIEngine.h"
#include "SoloScene.h"
#include "SoloDevice.h"
#include "SoloVideoDriver.h"

namespace solo
{
	class IEngineCallback;

	class Engine: public IEngine
	{
	public:
		Engine();
		virtual ~Engine();

		static Engine *getEngine()
		{
			static Engine instance;
			return &instance;
		}

		void run(const EngineCreationArgs &launchArgs) override;
		void setCallback(IEngineCallback* callback) override;

		virtual ptr<IScene> getScene() const override
		{
			return _scene;
		}

		virtual ptr<IDevice> getDevice() const override
		{
			return _device;
		}

		virtual ptr<IVideoDriver> getVideoDriver() const override
		{
			return _driver;
		}

		virtual float getTimeDelta() const override
		{
			return _timeDelta;
		}

	private:
		IEngineCallback *_callback;
		ptr<Scene> _scene;
		ptr<Device> _device;
		ptr<VideoDriver> _driver;

		unsigned long _lastUpdateTime;
		float _timeDelta;

		inline void _updateTime();
		void _run(const EngineCreationArgs &args);
	};
}

#endif