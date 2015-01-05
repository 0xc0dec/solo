#pragma once

#include "SoloIVideoDriver.h"
#include "SoloEffect.h"

namespace solo
{
	class VideoDriver: public IVideoDriver
	{
	public:
		static ptr<VideoDriver> create();

		virtual ptr<IEffect> createEffect(const std::string &vsSrc, const std::string &fsSrc) override;

		virtual void setViewport(float left, float top, float width, float height) = 0;

		virtual void setClearColor(float r, float g, float b, float a) = 0;

		virtual void clear() = 0;

	protected:
		explicit VideoDriver();
		~VideoDriver();

		std::list<ptr<Effect>> _effects;
	};
}
