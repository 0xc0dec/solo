#ifndef __SOLO_DEVICE_H__
#define __SOLO_DEVICE_H__

#include <list>
#include "SoloIDevice.h"
#include "SoloIEngine.h"
#include "SoloCommonsInternal.h"

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

		std::list<sptr<GPUProgram>> _gpuPrograms;
	};
}

#endif