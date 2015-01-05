#pragma once

#include "SoloBase.h"

namespace solo
{
	// A wrap around a GPU program
	class IEffect
	{
	public:
		virtual ~IEffect() {}

		virtual size_t getId() const = 0;

		virtual bool isValid() const = 0;
		virtual std::string getLog() const = 0;
	};
}
