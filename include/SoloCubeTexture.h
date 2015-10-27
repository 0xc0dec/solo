#pragma once

#include "SoloTexture.h"
#include "SoloImage.h"

namespace solo
{
	enum class CubeTextureFace
	{
		Front = 0,
		Back = 1,
		Left = 2,
		Right = 3,
		Top = 4,
		Bottom = 5
	};

	class CubeTexture: public Texture
	{
	public:
		virtual void setData(CubeTextureFace face, ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height) = 0;

		virtual void generateMipmaps() = 0;

		TextureWrapMode getDepthWrapMode() const;
		void setDepthWrapMode(TextureWrapMode depthWrap);

	protected:
		CubeTexture() {}

		TextureWrapMode depthWrap = TextureWrapMode::Repeat;
	};
}