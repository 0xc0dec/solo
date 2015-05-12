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
		virtual void setData(ColorFormat format, const std::vector<byte> &data, unsigned width, unsigned height) override;
		virtual void generateMipmaps() override;

	private:
		OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D(OpenGLTexture2D&& other) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D& operator=(OpenGLTexture2D&& other) = delete;

		GLenum toGLColorFormat(ColorFormat format);
		GLenum toGLWrapMode(WrapMode mode);
		GLenum toGLFilter(Filter filter);

		GLuint handle = 0;
	};
}