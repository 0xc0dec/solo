#pragma once

#include "SoloEngine.h"
#include "SoloVector2.h"

namespace solo
{
	class Device
	{
	public:
		virtual ~Device() {}

		static ptr<Device> create(const EngineCreationArgs& args);

		virtual void setWindowTitle(const char *title) = 0;

		virtual Vector2 getCanvasSize() const = 0;

		/// Returns time elapsed since the engine startup in ms
		virtual unsigned long getLifetime() const = 0;

		virtual void beginUpdate() = 0;
		virtual void endUpdate() = 0;

		bool closeRequested() const;

	protected:
		Device(const EngineCreationArgs& args);

		bool close;
		EngineCreationArgs creationArgs;
	};
}
