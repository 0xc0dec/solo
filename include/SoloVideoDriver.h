#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect;
	class Mesh;

	class VideoDriver
	{
	public:
		virtual ~VideoDriver() {}

		static ptr<VideoDriver> create();

		virtual void drawMesh(ptr<Mesh> mesh) = 0;

		virtual ptr<Effect> createEffect(const std::string &vsSrc, const std::string &fsSrc);

		virtual void setViewport(float left, float top, float width, float height) = 0;

		virtual void setClearColor(float r, float g, float b, float a) = 0;

		virtual void clear() = 0;

	protected:
		VideoDriver() {}

		std::list<ptr<Effect>> _effects;
	};
}
