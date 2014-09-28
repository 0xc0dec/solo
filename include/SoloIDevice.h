#ifndef __SOLO_IDEVICE_H__
#define __SOLO_IDEVICE_H__

#include "SoloCommons.h"

namespace solo
{
	class IDevice
	{
	public:
		virtual ~IDevice() {}

		virtual void setWindowTitle(const c8 *caption) = 0;
	};
}

#endif