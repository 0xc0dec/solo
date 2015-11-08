#pragma once

#include "SoloGraphics.h"

namespace solo
{
	class StubGraphics: public Graphics
	{
	public:
		virtual void renderMaterialSurfaceToTarget(Material* material, RenderTarget* target) override {}
		virtual void renderMaterialSurfaceToScreen(Material* material) override {}

	private:
		friend class Graphics;

		explicit StubGraphics(Device *device): Graphics(device) {}
	};
}