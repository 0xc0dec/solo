#ifndef __SOLO_I_EFFECT_H__
#define __SOLO_I_EFFECT_H__

#include "SoloBase.h"

namespace solo
{
	class IEffect
	{
	public:
		virtual ~IEffect() {}

		virtual size_t getId() const = 0;

		virtual bool isValid() const = 0;
		virtual std::string getLog() const = 0;
	};
}

#endif