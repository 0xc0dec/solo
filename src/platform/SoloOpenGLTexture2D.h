#pragma once

#include <GL/glew.h>
#include "SoloTexture2D.h"

namespace solo
{
	class OpenGLTexture2D: public Texture2D
	{
	public:
		virtual ~OpenGLTexture2D();

	private:
		friend class TextureFactory;

		OpenGLTexture2D(Format format, std::vector<char> data, unsigned width, unsigned height, bool generateMipmaps);
		OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D(OpenGLTexture2D&& other) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D& operator=(OpenGLTexture2D&& other) = delete;

		GLint toGLFormat(Format format);

		GLuint handle;
	};
}