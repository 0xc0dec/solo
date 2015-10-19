#pragma once

#include "SoloTexture.h"
#include "SoloImage.h"

namespace solo
{
	class TextureCube: public Texture
	{
	public:
		void setFrontImageData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height);
		void setBackImageData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height);
		void setLeftImageData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height);
		void setRightImageData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height);
		void setTopImageData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height);
		void setBottomImageData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height);

		virtual void generateMipmaps() = 0;

		TextureWrapMode getDepthWrapMode() const;
		void setDepthWrapMode(TextureWrapMode depthWrap);

	protected:
		TextureCube() {}

	private:
		TextureWrapMode depthWrap = TextureWrapMode::Repeat;
	};
}