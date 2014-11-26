#ifndef __SOLO_DEVICE_H__
#define __SOLO_DEVICE_H__

#include "SoloIDevice.h"
#include "SoloIEngine.h"
#include "SoloBaseInternal.h"

namespace solo
{
	class GPUProgram;

	class Device: public IDevice
	{
	public:
		explicit Device(EngineCreationArgs args);
		~Device();

		bool closeRequested() const { return _closeRequested; }

		/// Returns time elapsed since the engine startup in ms
		virtual u64 lifetime() const = 0;
		virtual void update() = 0;

	protected:
		bool _closeRequested;
		EngineCreationArgs _creationArgs;

		List<ptr<GPUProgram>> _gpuPrograms;
	};
}

#endif