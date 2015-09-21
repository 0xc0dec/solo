#pragma once

#include "SoloMaterial.h"

namespace solo
{
	class OpenGLMaterial: public Material
	{
	protected:
		virtual void applyFaceCull() override;

	private:
		friend class MaterialFactory;

		OpenGLMaterial(shared<Effect> effect);
	};
}