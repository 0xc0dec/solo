#ifndef __SOLO_I_DEVICE_H__
#define __SOLO_I_DEVICE_H__

#include "SoloVector2.h"

namespace solo
{
	class IDevice
	{
	public:
		virtual ~IDevice() {}

		virtual void setWindowTitle(const char *title) = 0;

		virtual Vector2 getCanvasSize() const = 0;
	};
}

#endif