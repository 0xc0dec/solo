#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;

	class Texture
	{
	public:
		enum class Format
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
			LineapMipmapNearest,
			NearestMipmapLinear,
			LinearMipmapLinear
		};

		virtual ~Texture() {}

		void setWrapMode(WrapMode verticalWrap, WrapMode horizontalWrap);
		void setFilterMode(Filter minFilter, Filter magFilter);

		virtual void apply() = 0;

	protected:
		Texture() {}

		WrapMode verticalWrap;
		WrapMode horizontalWrap;
		Filter minFilter;
		Filter magFilter;

	private:
		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;
	};

	class TextureFactory
	{
		static shared<Texture2D> create2D(Texture::Format format, std::vector<char> data, unsigned width, unsigned height, bool generateMipmaps);
	};
}