#pragma once

#include "SoloTexture.h"
#include "SoloVector2.h"

namespace solo
{
	enum class TextureWrapMode
	{
		Clamp,
		Repeat
	};

	enum class TextureFilter
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
		void setData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height);
		virtual void generateMipmaps() = 0;

		Vector2 getSize() const;

		TextureWrapMode getVerticalWrapMode() const;
		TextureWrapMode getHorizontalWrapMode() const;
		void setWrapMode(TextureWrapMode verticalWrap, TextureWrapMode horizontalWrap);

		TextureFilter getMinFilter() const;
		TextureFilter getMagFilter() const;
		void setFilterMode(TextureFilter minFilter, TextureFilter magFilter);

		float getAnisotropyLevel() const;
		void setAnisotropyLevel(float level);

	protected:
		Texture2D() {}

		virtual void applyData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height) = 0;

		TextureWrapMode verticalWrap = TextureWrapMode::Repeat;
		TextureWrapMode horizontalWrap = TextureWrapMode::Repeat;
		TextureFilter minFilter = TextureFilter::Linear;
		TextureFilter magFilter = TextureFilter::Linear;
		float anisotropy = 1.0f;

	private:
		Vector2 size;
	};
}