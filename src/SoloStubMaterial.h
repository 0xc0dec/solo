#pragma once

#include "SoloMaterial.h"

namespace solo
{
	class StubMaterial: public Material
	{
	protected:
		virtual void applyFaceCull() override {}

	private:
		friend class Material;

		explicit StubMaterial(const shared<Effect>& effect)
			: Material(effect)
		{
		}
	};
}