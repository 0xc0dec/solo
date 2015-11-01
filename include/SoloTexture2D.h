#pragma once

#include "SoloTexture.h"
#include "SoloVector2.h"
#include "SoloImage.h"

namespace solo
{
	class Texture2D: public Texture
	{
	public:
		void setData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height);

		virtual void generateMipmaps() = 0;

		Vector2 getSize() const;

	protected:
		Texture2D() {}

		virtual void applyData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height) = 0;

	private:
		Vector2 size;
	};

	inline Vector2 Texture2D::getSize() const
	{
		return size;
	}
}