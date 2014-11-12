#ifndef __SOLO_I_DEVICE_H__
#define __SOLO_I_DEVICE_H__

#include "SoloCommon.h"

namespace solo
{
	class IGPUProgram;

	class IDevice
	{
	public:
		virtual ~IDevice() {}

		virtual void setWindowTitle(const c8 *title) = 0;
		virtual ptr<IGPUProgram> createGPUProgram(const String &vsSrc, const String &fsSrc) = 0;
	};
}

#endif