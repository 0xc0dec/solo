#pragma once

#include "SoloTexture.h"
#include "SoloVector2.h"

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
		void setData(ColorFormat format, const std::vector<byte> &data, unsigned width, unsigned height);
		virtual void generateMipmaps() = 0;

		Vector2 getSize() const;

		WrapMode getVerticalWrapMode() const;
		WrapMode getHorizontalWrapMode() const;
		void setWrapMode(WrapMode verticalWrap, WrapMode horizontalWrap);

		Filter getMinFilter() const;
		Filter getMagFilter() const;
		void setFilterMode(Filter minFilter, Filter magFilter);

		float getAnisotropyLevel() const;
		void setAnisotropyLevel(float level);

	protected:
		Texture2D() {}

		virtual void applyData(ColorFormat format, const std::vector<byte> &data, unsigned width, unsigned height) = 0;

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

		Vector2 size;
	};
}