#pragma once

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
		const char *windowTitle;

		EngineCreationArgs(
			EngineMode mode,
			int canvasWidth = 800,
			int canvasHeight = 600,
			int bits = 32,
			int depth = 16,
			bool fullScreen = false,
			const char *windowTitle = "Test"):
			mode {mode},
			canvasWidth {canvasWidth},
			canvasHeight {canvasHeight},
			bits {bits},
			depth {depth},
			fullScreen {fullScreen},
			windowTitle {windowTitle}
		{
		}
	};
}