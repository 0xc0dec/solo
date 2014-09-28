#ifndef __SOLO_DEVICE_H__
#define __SOLO_DEVICE_H__

#include "SoloIDevice.h"
#include "SoloIEngine.h"
#include "SoloCommonsInternal.h"

namespace solo
{
	class Device: public IDevice
	{
	public:
		explicit Device(EngineLaunchArgs args);
		~Device();

		bool closeRequested() const { return _closeRequested; }

		/// Returns time elapsed since the engine startup in ms
		virtual u64 lifetime() = 0;
		virtual void update() = 0;

	protected:
		bool _closeRequested;
	};
}

#endif