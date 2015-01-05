#pragma once

namespace solo
{
	// Encapsulates the shader parameters and logic required to render an object with that shader.
	class Pass
	{
	public:
		void bind();
		void unbind();
	};
}