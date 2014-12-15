#ifndef __SOLO_ENGINE_H__
#define __SOLO_ENGINE_H__

#include "SoloBaseInternal.h"
#include "SoloIEngine.h"
#include "SoloScene.h"
#include "SoloDevice.h"

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

		virtual f32 getDeltaTime() const override
		{
			return _deltaTime;
		}

	private:
		IEngineCallback *_callback;
		ptr<Scene> _scene;
		ptr<Device> _device;

		u64 _lastUpdateTime;
		f32 _deltaTime;

		void _run(const EngineCreationArgs &args);
	};
}

#endif