#ifndef __SOLO_IDEVICE_H__
#define __SOLO_IDEVICE_H__

namespace solo
{
	class IDevice
	{
	public:
		virtual ~IDevice() {}

		virtual void setWindowTitle(const char *caption) = 0;
	};
}

#endif