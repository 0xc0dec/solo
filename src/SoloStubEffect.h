#pragma once

#include "SoloEffect.h"

namespace solo
{
	class StubEffect: public Effect
	{
	public:
		virtual void bind() override {}

	private:
		friend class EffectFactory;

		StubEffect() {}
	};
}