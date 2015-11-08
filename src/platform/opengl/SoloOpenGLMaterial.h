#pragma once

#include "SoloMaterial.h"

namespace solo
{
	class OpenGLMaterial: public Material
	{
	protected:
		virtual void applyState() override;

	private:
		friend class MaterialFactory;

		explicit OpenGLMaterial(shared<Effect> effect);
	};
}