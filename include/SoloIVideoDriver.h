#pragma once

#include "SoloBase.h"

namespace solo
{
	class IEffect;

	class IVideoDriver
	{
	public:
		virtual ~IVideoDriver() {}

		virtual ptr<IEffect> createEffect(const std::string &vsSrc, const std::string &fsSrc) = 0;
	};
}
