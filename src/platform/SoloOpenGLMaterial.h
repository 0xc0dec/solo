#pragma once

#include "SoloMaterial.h"

namespace solo
{
	class OpenGLMaterial: public Material
	{
	public:
		OpenGLMaterial(shared<Effect> effect);

	protected:
		virtual void applyFaceCull() override;
	};
}