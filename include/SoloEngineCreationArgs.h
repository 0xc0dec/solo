#pragma once

#include "SoloBase.h"

namespace solo
{
	enum class EngineMode
	{
		Stub,
		OpenGL
	};

	struct EngineCreationArgs
	{
		EngineMode mode;
		int canvasWidth;
		int canvasHeight;
		int bits;
		int depth;
		bool fullScreen;
		std::string windowTitle;

		EngineCreationArgs(
			EngineMode mode = EngineMode::OpenGL,
			int canvasWidth = 800,
			int canvasHeight = 600,
			int bits = 32,
			int depth = 16,
			bool fullScreen = false,
			const std::string& windowTitle = ""):
			mode(mode),
			canvasWidth(canvasWidth > 0 ? canvasWidth : 1),
			canvasHeight(canvasHeight > 0 ? canvasHeight : 1),
			bits(bits > 0 ? bits : 32),
			depth(depth > 0 ? depth : 16),
			fullScreen(fullScreen),
			windowTitle(windowTitle)
		{
		}
	};
}