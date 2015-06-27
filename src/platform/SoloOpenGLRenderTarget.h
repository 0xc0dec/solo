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
		virtual void applyTextures(const std::vector<shared<Texture2D>>& textures) override;

	private:
		void checkStatus();
		void deleteDepthBuffer();

		GLuint handle{ 0 };
		GLuint depthBufferHandle{ 0 };
	};
}
