#pragma once

#include "SoloIDevice.h"
#include "SoloIEngine.h"

namespace solo
{
	class Device: public IDevice
	{
	public:
		explicit Device(EngineCreationArgs args);
		~Device();

		bool closeRequested() const { return _closeRequested; }

		/// Returns time elapsed since the engine startup in ms
		virtual unsigned long getLifetime() const = 0;

		virtual void beginUpdate() = 0;
		virtual void endUpdate() = 0;

	protected:
		bool _closeRequested;
		EngineCreationArgs _creationArgs;
	};
}
