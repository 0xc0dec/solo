#pragma once

#include "SoloBase.h"

namespace solo
{
	// Wrap around a uniform variable
	class EffectVariable
	{
	public:
		EffectVariable(const std::string &name);
		virtual ~EffectVariable() {}

		std::string getName() const;

	protected:
		std::string _name;
	};
}