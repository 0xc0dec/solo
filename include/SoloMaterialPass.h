#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect;

	class MaterialPass
	{
	public:
		~MaterialPass() {}

		static ptr<MaterialPass> create();

		void setEffect(ptr<Effect> effect);

		void bind();
		void unbind();

	private:
		MaterialPass();

		ptr<Effect> _effect;
	};
}