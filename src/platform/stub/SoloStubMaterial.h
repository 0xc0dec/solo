#pragma once

#include "SoloMaterial.h"

namespace solo
{
	class StubMaterial: public Material
	{
	protected:
		virtual void applyFaceCull() override {}
		virtual void applyZWrite() override {}

	private:
		friend class MaterialFactory;

		explicit StubMaterial(const shared<Effect>& effect)
			: Material(effect)
		{
		}
	};
}