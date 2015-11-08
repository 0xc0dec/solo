#pragma once

#include "SoloGraphics.h"

namespace solo
{
	class OpenGLGraphics: public Graphics
	{
	public:
		virtual void renderSurfaceToTarget(Material* material, RenderTarget* target) override;
		virtual void renderSurfaceToScreen(Material* material) override;

	private:
		friend class Graphics;

		explicit OpenGLGraphics(Device *device);

		void drawMaterial(Material* material, RenderTarget* target);

		shared<Mesh> quadMesh;
	};
}