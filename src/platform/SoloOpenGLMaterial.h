#pragma once

#include "SoloMaterial.h"

namespace solo
{
	class OpenGLMaterial: public Material
	{
	protected:
		void applyFaceCull() override;
	};
}