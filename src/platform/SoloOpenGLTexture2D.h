#pragma once

#include <GL/glew.h>
#include "SoloTexture2D.h"

namespace solo
{
	class OpenGLTexture2D: public Texture2D
	{
	public:
		OpenGLTexture2D();
		virtual ~OpenGLTexture2D();

		virtual void apply() override;
		virtual void generateMipmaps() override;
		virtual void *getNativeHandle() override;

	protected:
		virtual void applyData(ColorFormat format, const std::vector<byte>& data, unsigned width, unsigned height) override;

	private:
		OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D(OpenGLTexture2D&& other) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D& operator=(OpenGLTexture2D&& other) = delete;

		void bind();
		void unbind();

		GLenum toGLColorFormat(ColorFormat format);
		GLenum toGLWrapMode(WrapMode mode);
		GLenum toGLFilter(Filter filter);

		GLuint handle = 0;
	};
}