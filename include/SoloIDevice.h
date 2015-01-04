#ifndef __SOLO_I_DEVICE_H__
#define __SOLO_I_DEVICE_H__

namespace solo
{
	class IDevice
	{
	public:
		virtual ~IDevice() {}

		virtual void setWindowTitle(const char *title) = 0;
	};
}

#endif