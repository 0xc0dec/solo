#pragma once

#include "SoloTextureSampler.h"

namespace solo
{
	class OpenGLTextureSampler2D : public TextureSampler
	{
	public:
		OpenGLTextureSampler2D(shared<Texture> texture);

		virtual void apply(RenderContext& context) override;
	};
}