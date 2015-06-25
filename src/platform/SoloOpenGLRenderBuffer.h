#pragma once

#include <GL/glew.h>
#include "SoloRenderBuffer.h"

namespace solo
{
	class OpenGLRenderBuffer: public RenderBuffer
	{
	public:
		OpenGLRenderBuffer();

		virtual void bind() override;

	private:
		~OpenGLRenderBuffer();
		
		GLuint handle{0};
	};
}
