#pragma once

#include <GL/glew.h>
#include "SoloRenderTarget.h"

namespace solo
{
	class OpenGLRenderTarget: public RenderTarget
	{
	public:
		OpenGLRenderTarget();
		virtual ~OpenGLRenderTarget();

		virtual void bind() override;
		virtual void unbind() override;

	protected:
		virtual void update() override;

	private:
		void checkStatus();

		GLuint handle{ 0 };
		GLuint depthBufferHandle{ 0 };
	};
}
