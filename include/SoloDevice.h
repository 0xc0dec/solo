#pragma once

#include "SoloEngine.h"
#include "SoloVector2.h"

namespace solo
{
	enum class KeyCode
	{
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		Escape,
		LeftArrow,
		RightArrow,
		UpArrow,
		DownArrow,
		// ...
	};

	enum class MouseButton
	{
		Left,
		Middle,
		Right
	};

	class Device
	{
	public:
		virtual ~Device() {}

		virtual void setWindowTitle(const char *title) = 0;
		virtual std::string getWindowTitle() const = 0;

		bool isKeyPressed(KeyCode code, bool firstTimeOnly = false) const;
		bool isKeyReleased(KeyCode code) const;

		Vector2 getMouseMotion() const;
		bool isMouseButtonPressed(MouseButton button, bool firstTimeOnly = false) const;
		bool isMouseButtonReleased(MouseButton button) const;

		virtual void setCursorCaptured(bool captured) = 0;

		virtual Vector2 getCanvasSize() const = 0;

		/// Returns time elapsed since the engine startup in seconds
		virtual float getLifetime() const = 0;

		virtual void beginUpdate() = 0;
		virtual void endUpdate() = 0;

		void requestShutdown();
		bool shutdownRequested() const;

	protected:
		Device(const EngineCreationArgs& args);

		// stores what keys were pressed and if it was a repeat
		std::map<KeyCode, bool> pressedKeys;
		std::set<KeyCode> releasedKeys;

		int mouseDeltaX;
		int mouseDeltaY;
		std::map<MouseButton, bool> pressedMouseButtons;
		std::set<MouseButton> releasedMouseButtons;

		bool close = false;
		EngineCreationArgs creationArgs;

	private:
		Device(const Device& other) = delete;
		Device(Device&& device) = delete;
		Device& operator=(const Device& other) = delete;
		Device& operator=(const Device&& other) = delete;
	};

	class DeviceFactory
	{
		friend class Engine;
		static shared<Device> create(const EngineCreationArgs& args);
	};
}
