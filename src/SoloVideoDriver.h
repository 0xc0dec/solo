#pragma once

#include "SoloIVideoDriver.h"

namespace solo
{
	class Effect;

	class VideoDriver: public IVideoDriver
	{
	public:
		virtual void setViewport(float left, float top, float width, float height) = 0;

		virtual void setClearColor(float r, float g, float b, float a) = 0;

		virtual void clear() = 0;

	protected:
		std::list<ptr<Effect>> _effects;
	};
}
