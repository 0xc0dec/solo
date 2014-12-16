#ifndef __SOLO_I_DEVICE_H__
#define __SOLO_I_DEVICE_H__

#include "SoloBase.h"

namespace solo
{
	class IGPUProgram;

	class IDevice
	{
	public:
		virtual ~IDevice() {}

		virtual void setWindowTitle(const char *title) = 0;
		virtual ptr<IGPUProgram> createGPUProgram(const std::string &vsSrc, const std::string &fsSrc) = 0;
	};
}

#endif