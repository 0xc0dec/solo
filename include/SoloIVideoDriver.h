#ifndef __SOLO_I_VIDEO_DRIVER_H__
#define __SOLO_I_VIDEO_DRIVER_H__

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

#endif