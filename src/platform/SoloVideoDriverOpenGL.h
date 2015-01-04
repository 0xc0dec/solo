#ifndef __SOLO_VIDEO_DRIVER_OPENGL_H__
#define __SOLO_VIDEO_DRIVER_OPENGL_H__

#include "../SoloVideoDriver.h"

namespace solo
{
	class VideoDriverOpenGL : public VideoDriver
	{
	public:
		virtual ptr<IGPUProgram> createGPUProgram(const std::string &vsSrc, const std::string &fsSrc) override;

		virtual void setViewport(int left, int top, int width, int height) override;
	};
}

#endif