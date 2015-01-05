#pragma once

#include "SoloIDevice.h"
#include "SoloIEngine.h"

namespace solo
{
	class Device: public IDevice
	{
	public:
		static ptr<Device> create(const EngineCreationArgs& args);

		bool closeRequested() const;

		/// Returns time elapsed since the engine startup in ms
		virtual unsigned long getLifetime() const = 0;

		virtual void beginUpdate() = 0;
		virtual void endUpdate() = 0;

	protected:
		Device(const EngineCreationArgs& args);
		~Device() {}

		bool _closeRequested;
		EngineCreationArgs _creationArgs;
	};
}
