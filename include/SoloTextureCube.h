#pragma once

#include "SoloTexture.h"

namespace solo
{
	class TextureCube: public Texture
	{
	public:
		virtual void generateMipmaps() = 0;

		TextureWrapMode getDepthWrapMode() const;
		void setDepthWrapMode(TextureWrapMode depthWrap);

	protected:
		TextureCube() {}

	private:
		TextureWrapMode depthWrap = TextureWrapMode::Repeat;
	};
}