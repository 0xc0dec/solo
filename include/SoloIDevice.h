#ifndef __SOLO_I_DEVICE_H__
#define __SOLO_I_DEVICE_H__

#include "SoloCommons.h"

namespace solo
{
	class IGPUProgram;

	class IDevice
	{
	public:
		virtual ~IDevice() {}

		virtual void setWindowTitle(const c8 *title) = 0;
		virtual sptr<IGPUProgram> createGPUProgram(const str &vsSrc, const str &fsSrc) = 0;
	};
}

#endif