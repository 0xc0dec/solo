#ifndef __SOLO_OPENGL_VIDEO_DRIVER_H__
#define __SOLO_OPENGL_VIDEO_DRIVER_H__

#include "../SoloVideoDriver.h"

namespace solo
{
	class OpenGLVideoDriver : public VideoDriver
	{
	public:
		virtual ptr<IEffect> createEffect(const std::string &vsSrc, const std::string &fsSrc) override;

		virtual void setViewport(float left, float top, float width, float height) override;

		virtual void setClearColor(float r, float g, float b, float a) override;

		virtual void clear() override;
	};
}

#endif