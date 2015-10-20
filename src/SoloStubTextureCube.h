#pragma once

#include "SoloTextureCube.h"

namespace solo
{
	class StubTextureCube : public TextureCube
	{
	public:
		virtual void setImageData(TextureCubeFace face, ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height) override {}
		virtual void apply() override {}
		virtual void generateMipmaps() override {}

	private:
		friend class TextureFactory;

		StubTextureCube() {}
	};
}