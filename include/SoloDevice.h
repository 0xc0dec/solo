#pragma once

#include "SoloEngine.h"
#include "SoloVector2.h"

namespace solo
{
	enum class KeyCode
	{
		Escape,
		LeftArrow,
		RightArrow,
		UpArrow,
		DownArrow,
		// ...
	};

	class Device
	{
	public:
		virtual ~Device() {}

		virtual void setWindowTitle(const char *title) = 0;
		virtual std::string getWindowTitle() const = 0;

		virtual bool isKeyPressed(KeyCode code, bool repeat = false) const;
		virtual bool isKeyReleased(KeyCode code) const;

		virtual Vector2 getCanvasSize() const = 0;

		/// Returns time elapsed since the engine startup in ms
		virtual unsigned long getLifetime() const = 0;

		virtual void beginUpdate() = 0;
		virtual void endUpdate() = 0;

		void requestShutdown();
		bool shutdownRequested() const;

	protected:
		friend class DeviceFactory;

		Device(const EngineCreationArgs& args);
		Device(const Device& other) = delete;
		Device(Device&& device) = delete;
		Device& operator=(const Device& other) = delete;
		Device& operator=(const Device&& other) = delete;

		// stores what keys were pressed and if it was a repeat
		std::unordered_map<KeyCode, bool> pressedKeys;
		std::unordered_set<KeyCode> releasedKeys;
		
		bool close;
		EngineCreationArgs creationArgs;
	};

	class DeviceFactory
	{
		friend class Engine;
		static shared<Device> create(const EngineCreationArgs& args);
	};
}
