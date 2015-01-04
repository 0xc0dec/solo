#ifndef __SOLO_VIDEO_DRIVER_OPENGL_H__
#define __SOLO_VIDEO_DRIVER_OPENGL_H__

#include "../SoloVideoDriver.h"

namespace solo
{
	class VideoDriverOpenGL : public VideoDriver
	{
	public:
		ptr<IGPUProgram> createGPUProgram(const std::string &vsSrc, const std::string &fsSrc) override;
	};
}

#endif