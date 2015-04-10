#pragma once

#include <GL/glew.h>
#include "SoloTexture2D.h"

namespace solo
{
	class OpenGLTexture2D: public Texture2D
	{
	public:
		virtual ~OpenGLTexture2D();

		virtual void apply() override;

	private:
		friend class TextureFactory;

		OpenGLTexture2D(ColorFormat format, std::vector<byte> data, unsigned width, unsigned height, bool generateMipmaps);
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