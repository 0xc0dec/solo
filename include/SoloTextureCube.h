#pragma once

#include "SoloTexture.h"
#include "SoloImage.h"

namespace solo
{
	enum class TextureCubeFace
	{
		Front = 0,
		Back = 1,
		Left = 2,
		Right = 4,
		Top = 5,
		Bottom = 6
	};

	class TextureCube: public Texture
	{
	public:
		virtual void setData(TextureCubeFace face, ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height) = 0;

		virtual void generateMipmaps() = 0;

		TextureWrapMode getDepthWrapMode() const;
		void setDepthWrapMode(TextureWrapMode depthWrap);

	protected:
		TextureCube() {}

		TextureWrapMode depthWrap = TextureWrapMode::Repeat;
	};
}