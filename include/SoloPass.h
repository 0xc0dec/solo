#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect;

	class Pass
	{
	public:
		~Pass() {}

		static ptr<Pass> create();

		void setEffect(ptr<Effect> effect);

		void bind();
		void unbind();

	private:
		Pass();

		ptr<Effect> _effect;
	};
}