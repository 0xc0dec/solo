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

		virtual void setTexture(shared<Texture2D> texture) override;

	private:
		void checkStatus();
		void deleteDepthBuffer();

		GLuint handle{ 0 };
		GLuint depthBufferHandle{ 0 };
	};
}
