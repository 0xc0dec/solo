#ifndef __SOLO_ENGINE_H__
#define __SOLO_ENGINE_H__

#include "SoloCommonsInternal.h"
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

		virtual sptr<IScene> scene() const override
		{
			return _scene;
		}

		virtual sptr<IDevice> device() const override
		{
			return _device;
		}

		virtual f32 deltaTime() const override
		{
			return _deltaTime;
		}

	private:
		IEngineCallback *_callback;
		sptr<Scene> _scene;
		sptr<Device> _device;

		u64 _lastUpdateTime;
		f32 _deltaTime;

		void _run(const EngineCreationArgs &args);
	};
}

#endif