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
		bool fullScreen;
		std::string windowTitle;
		int bits;
		int depth;

		EngineCreationArgs(
			EngineMode mode = EngineMode::OpenGL,
			int canvasWidth = 800,
			int canvasHeight = 600,
			bool fullScreen = false,
			const std::string& windowTitle = "",
			int bits = 32,
			int depth = 16):
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
}