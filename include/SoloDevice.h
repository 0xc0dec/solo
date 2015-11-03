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
		Device(const Device& other) = delete;
		Device(Device&& device) = delete;
		Device& operator=(const Device& other) = delete;
		Device& operator=(Device&& other) = delete;
		virtual ~Device() {}

		virtual void beginUpdate() = 0;
		virtual void endUpdate() = 0;

		virtual void setWindowTitle(const std::string &title) = 0;
		virtual std::string getWindowTitle() const = 0;

		virtual void setCursorCaptured(bool captured) = 0;

		virtual Vector2 getCanvasSize() const = 0;

		virtual float getLifetime() const = 0;
		
		float getTimeDelta() const;

		bool isKeyPressed(KeyCode code, bool firstTimeOnly = false) const;
		bool isKeyReleased(KeyCode code) const;

		Vector2 getMouseMotion() const;
		bool isMouseButtonDown(MouseButton button, bool firstTimeOnly = false) const;
		bool isMouseButtonReleased(MouseButton button) const;

		Engine* getEngine() const;

	protected:
		explicit Device(Engine *engine, const EngineCreationArgs& args);

		void updateTime();

		// stores what keys were pressed and if it was a repeat
		std::unordered_map<KeyCode, bool> pressedKeys;
		std::unordered_set<KeyCode> releasedKeys;

		int mouseDeltaX = 0;
		int mouseDeltaY = 0;
		std::unordered_map<MouseButton, bool> pressedMouseButtons;
		std::unordered_set<MouseButton> releasedMouseButtons;

		Engine *engine;
		bool close = false;
		float lastUpdateTime = 0;
		float timeDelta = 0;
		EngineCreationArgs creationArgs;
	};

	inline float Device::getTimeDelta() const
	{
		return timeDelta;
	}

	inline Engine* Device::getEngine() const
	{
		return engine;
	}

	class DeviceFactory
	{
		friend class Engine;
		static shared<Device> create(Engine *engine, const EngineCreationArgs& args);
	};
}
