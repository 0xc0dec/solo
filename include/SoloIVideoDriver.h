#pragma once

#include "SoloBase.h"
#include "SoloIEffect.h"

namespace solo
{
	class IVideoDriver
	{
	public:
		virtual ~IVideoDriver() {}

		virtual ptr<IEffect> createEffect(const std::string &vsSrc, const std::string &fsSrc) = 0;
	};
}
