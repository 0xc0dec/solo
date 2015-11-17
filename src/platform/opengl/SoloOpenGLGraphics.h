#pragma once

#include "SoloGraphics.h"

namespace solo
{
	class OpenGLGraphics: public Graphics
	{
	public:
		virtual void renderSurface(Material* material, RenderTarget* target) override;

	private:
		friend class Graphics;

		explicit OpenGLGraphics(Device *device);

		shared<Mesh> quadMesh;
	};
}