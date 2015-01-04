#ifndef __SOLO_DEVICE_H__
#define __SOLO_DEVICE_H__

#include "SoloIDevice.h"
#include "SoloIEngine.h"

namespace solo
{
	class Device: public IDevice
	{
	public:
		explicit Device(EngineCreationArgs args);
		~Device();

		bool closeRequested() const { return _closeRequested; }

		/// Returns time elapsed since the engine startup in ms
		virtual unsigned long getLifetime() const = 0;
		virtual void update() = 0;

	protected:
		bool _closeRequested;
		EngineCreationArgs _creationArgs;
	};
}

#endif