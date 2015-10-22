#pragma once

#include "SoloMaterial.h"

namespace solo
{
	class OpenGLMaterial: public Material
	{
	protected:
		virtual void applyFaceCull() override;
		virtual void applyZWrite() override;

	private:
		friend class MaterialFactory;

		explicit OpenGLMaterial(shared<Effect> effect);
	};
}