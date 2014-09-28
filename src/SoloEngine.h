#ifndef __SOLO_ENGINE_H__
#define __SOLO_ENGINE_H__

#include "SoloCommonsInternal.h"
#include "SoloIEngine.h"
#include "SoloScene.h"
#include "SoloDevice.h"
#include "SoloEmptyEngineCallback.h"

namespace solo
{
	class IEngineCallback;

	class Engine: public IEngine
	{
	public:
		Engine();
		~Engine();

		static Engine *getEngine()
		{
			static Engine instance;
			return &instance;
		}

		void run(const EngineLaunchArgs &launchArgs) override;
		void setCallback(IEngineCallback* callback) override;

		sptr<IScene> getScene() const override
		{
			return _scene;
		}

		sptr<IDevice> getDevice() const override
		{
			return _device;
		}

	private:
		IEngineCallback *_callback;
		sptr<Scene> _scene;
		sptr<Device> _device;
		EmptyEngineCallback _emptyCallback;

		unsigned long _lastUpdateTime;

		void _run(const EngineLaunchArgs & args);
	};
}

#endif