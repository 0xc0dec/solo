#pragma once

#include "SoloEffect.h"

namespace solo
{
	class StubEffectVariable;

	class StubEffect: public Effect
	{
	public:
		virtual void bind() override {}

		virtual EffectVariable* findVariable(const std::string& name) const override;

	private:
		friend class EffectFactory;

		StubEffect();

		shared<EffectVariable> variable;
	};
}