#pragma once

#include "SoloDevice.h"

namespace solo
{
	class StubDevice: public Device
	{
	public:
		virtual void beginUpdate() override;
		virtual void endUpdate() override;

		virtual void setWindowTitle(const std::string& title) override;
		virtual std::string getWindowTitle() const override;

		virtual void setCursorCaptured(bool captured) override;

		virtual Vector2 getCanvasSize() const override;

		virtual float getLifetime() const override;

	private:
		friend class DeviceFactory;

		explicit StubDevice(const EngineCreationArgs& args)
			: Device(args)
		{
		}

		std::string windowTitle;
	};
}