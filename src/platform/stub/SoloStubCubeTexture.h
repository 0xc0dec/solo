#pragma once

#include "SoloCubeTexture.h"

namespace solo
{
	class StubCubeTexture: public CubeTexture
	{
	public:
		virtual void setData(CubeTextureFace face, ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height) override {}
		virtual void apply() override {}
		virtual void generateMipmaps() override {}

	private:
		friend class Texture;

		StubCubeTexture() {}
	};
}