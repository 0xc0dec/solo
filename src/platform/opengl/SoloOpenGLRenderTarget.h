#pragma once

#include "SoloRenderTarget.h"
#include <GL/glew.h>

namespace solo
{
	class OpenGLRenderTarget: public RenderTarget
	{
	public:
		virtual ~OpenGLRenderTarget();

		virtual void bind() override;
		virtual void unbind() override;

	protected:
		virtual void update() override;

	private:
		friend class RenderTargetFactory;

		OpenGLRenderTarget();

		void checkStatus();

		GLuint handle{ 0 };
		GLuint depthBufferHandle{ 0 };
	};
}
