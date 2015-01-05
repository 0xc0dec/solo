#pragma once

#include "SoloBaseInternal.h"
#include "SoloIMaterial.h"

namespace solo
{
	class Material: public IMaterial
	{
	public:
		static ptr<Material> create();

	private:
		Material() {}
		virtual ~Material() override {}
	};
}
