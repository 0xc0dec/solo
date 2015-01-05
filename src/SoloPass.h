#pragma once

#include "SoloBaseInternal.h"

namespace solo
{
	// Encapsulates shader parameters and logic required to render an object with that shader.
	class Pass
	{
	public:
		static ptr<Pass> create();

		void bind();
		void unbind();

	private:
		Pass() {}
		virtual ~Pass() {}
	};
}