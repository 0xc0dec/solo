#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;

	class Texture
	{
	public:
		enum class ColorFormat
		{
			RGB,
			RGBA
		};

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

		virtual ~Texture() {}

		void setWrapMode(WrapMode verticalWrap, WrapMode horizontalWrap);
		void setFilterMode(Filter minFilter, Filter magFilter);

		virtual void apply() = 0;

	protected:
		Texture() {}

		WrapMode verticalWrap = WrapMode::Repeat;
		WrapMode horizontalWrap = WrapMode::Repeat;
		Filter minFilter = Filter::Linear;
		Filter magFilter = Filter::Linear;

	private:
		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;
	};

	class TextureFactory
	{
		friend class PNGTextureLoader;
		static shared<Texture2D> create2D(Texture::ColorFormat format, std::vector<byte> data, unsigned width, unsigned height, bool generateMipmaps);
	};
}