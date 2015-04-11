#pragma once

namespace solo
{
	struct EngineCreationArgs
	{
		int canvasWidth;
		int canvasHeight;
		int bits;
		int depth;
		bool fullScreen;
		const char *windowTitle;

		EngineCreationArgs(int canvasWidth = 800, int canvasHeight = 600, int bits = 32, int depth = 16, bool fullScreen = false, const char *windowTitle = "Test"):
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