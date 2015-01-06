#pragma once

#include "SoloBase.h"

namespace solo
{
	// Encapsulates shader parameters and logic required to render an object with that shader.
	class Pass
	{
	public:
		~Pass() {}

		static ptr<Pass> create();

	private:
		Pass() {}
	};
}