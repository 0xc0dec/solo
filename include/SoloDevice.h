#pragma once

#include "SoloBase.h"
#include "SoloVector2.h"

namespace solo
{
	class Scene;
	class ResourceManager;
	class FileSystem;
	class Graphics;

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

	enum class DeviceMode
	{
		Stub,
		OpenGL
	};

	struct DeviceCreationArgs
	{
		DeviceMode mode;
		int canvasWidth;
		int canvasHeight;
		bool fullScreen;
		std::string windowTitle;
		int bits;
		int depth;

		DeviceCreationArgs(
			DeviceMode mode = DeviceMode::OpenGL,
			int canvasWidth = 800,
			int canvasHeight = 600,
			bool fullScreen = false,
			const std::string& windowTitle = "",
			int bits = 32,
			int depth = 16) :
			mode(mode),
			canvasWidth(canvasWidth > 0 ? canvasWidth : 1),
			canvasHeight(canvasHeight > 0 ? canvasHeight : 1),
			fullScreen(fullScreen),
			windowTitle(windowTitle),
			bits(bits > 0 ? bits : 32),
			depth(depth > 0 ? depth : 16)
		{
		}
	};

	class Device
	{
	public:
		static shared<Device> create(const DeviceCreationArgs &args);

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

		void run();

		void setStartCallback(std::function<void()> callback);
		void setShutdownCallback(std::function<void()> callback);
		void setShutdownRequestedCallback(std::function<bool()> callback);

		void requestShutdown();
		bool shutdownRequested() const;

		DeviceMode getMode() const;

		Scene *getScene() const;
		FileSystem *getFileSystem() const;
		ResourceManager *getResourceManager() const;

		Graphics* getGraphics() const;

	protected:
		explicit Device(const DeviceCreationArgs& args);

		void updateTime();

		bool shutdown = false;
		DeviceCreationArgs creationArgs;
		std::function<void()> startCallback{ [] {} };
		std::function<void()> shutdownCallback{ [] {} };
		std::function<bool()> shutdownRequestedCallback{ [] { return true; } };

		shared<Scene> scene;
		shared<FileSystem> fs;
		shared<ResourceManager> resourceManager;
		shared<Graphics> graphics;

		// stores what keys were pressed and if it was a repeat
		std::unordered_map<KeyCode, bool> pressedKeys;
		std::unordered_set<KeyCode> releasedKeys;

		int mouseDeltaX = 0;
		int mouseDeltaY = 0;
		std::unordered_map<MouseButton, bool> pressedMouseButtons;
		std::unordered_set<MouseButton> releasedMouseButtons;

		bool close = false;
		float lastUpdateTime = 0;
		float timeDelta = 0;
	};

	inline float Device::getTimeDelta() const
	{
		return timeDelta;
	}

	inline void Device::requestShutdown()
	{
		shutdown = true;
	}

	inline bool Device::shutdownRequested() const
	{
		return shutdown;
	}

	inline DeviceMode Device::getMode() const
	{
		return creationArgs.mode;
	}

	inline Scene *Device::getScene() const
	{
		return scene.get();
	}

	inline FileSystem *Device::getFileSystem() const
	{
		return fs.get();
	}

	inline ResourceManager *Device::getResourceManager() const
	{
		return resourceManager.get();
	}

	inline Graphics* Device::getGraphics() const
	{
		return graphics.get();
	}

	inline void Device::setStartCallback(std::function<void()> callback)
	{
		startCallback = callback;
	}

	inline void Device::setShutdownCallback(std::function<void()> callback)
	{
		shutdownCallback = callback;
	}

	inline void Device::setShutdownRequestedCallback(std::function<bool()> callback)
	{
		shutdownRequestedCallback = callback;
	}
}
