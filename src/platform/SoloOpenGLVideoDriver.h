#pragma once

#include "SoloVideoDriver.h"

namespace solo
{
	class OpenGLVideoDriver : public VideoDriver
	{
	public:
		virtual ~OpenGLVideoDriver() override;

		static ptr<OpenGLVideoDriver> create();

		virtual void setViewport(float left, float top, float width, float height) override;

		virtual void setClearColor(float r, float g, float b, float a) override;

		virtual void clear() override;

	private:
		OpenGLVideoDriver();
	};
}
