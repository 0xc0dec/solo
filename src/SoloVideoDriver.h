#ifndef __SOLO_VIDEO_DRIVER_H__
#define __SOLO_VIDEO_DRIVER_H__

#include "SoloIVideoDriver.h"

namespace solo
{
	class GPUProgram;

	class VideoDriver: public IVideoDriver
	{
	public:
		virtual void setViewport(int left, int top, int width, int height) = 0;

	protected:
		std::list<ptr<GPUProgram>> _gpuPrograms;
	};
}

#endif