#pragma once

#include "SoloTexture.h"

namespace solo
{
	enum class WrapMode
	{
		Clamp,
		Repeat
	};

	enum class Filter
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapLinear
	};

	class Texture2D: public Texture
	{
	public:
		virtual void setData(ColorFormat format, const std::vector<byte> &data, unsigned width, unsigned height) = 0;

		virtual void generateMipmaps() = 0;

		void setWrapMode(WrapMode verticalWrap, WrapMode horizontalWrap);
		void setFilterMode(Filter minFilter, Filter magFilter);
		void setAnisotropyLevel(float level);

	protected:
		Texture2D() {}

		WrapMode verticalWrap = WrapMode::Repeat;
		WrapMode horizontalWrap = WrapMode::Repeat;
		Filter minFilter = Filter::Linear;
		Filter magFilter = Filter::Linear;
		float anisotropy = 1.0f;

	private:
		Texture2D(const Texture2D& other) = delete;
		Texture2D(Texture2D&& other) = delete;
		Texture2D& operator=(const Texture2D& other) = delete;
		Texture2D& operator=(Texture2D&& other) = delete;
	};
}