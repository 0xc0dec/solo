#ifndef __SOLO_I_VIDEO_DRIVER_H__
#define __SOLO_I_VIDEO_DRIVER_H__

#include "SoloBase.h"

namespace solo
{
	class IGPUProgram;

	class IVideoDriver
	{
	public:
		virtual ~IVideoDriver() {}

		virtual ptr<IGPUProgram> createGPUProgram(const std::string &vsSrc, const std::string &fsSrc) = 0;
	};
}

#endif