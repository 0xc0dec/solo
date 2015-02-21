#include "SoloOpenGLTexture2D.h"

using namespace solo;

OpenGLTexture2D::OpenGLTexture2D(Format format, std::vector<char> data, unsigned width, unsigned height, bool generateMipmaps):
	handle(0)
{
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, toGLFormat(format), width, height, 0, toGLFormat(format), GL_UNSIGNED_BYTE, &data[0]);
	// TODO mipmaps, filtering...
	glBindTexture(GL_TEXTURE_2D, 0);
}


OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &handle);
}


GLint OpenGLTexture2D::toGLFormat(Format format)
{
	switch (format)
	{
		case Format::RGB:
			return GL_RGB;
		case Format::RGBA:
			return GL_RGBA;
		default:
			THROW(EngineException, "Unexpected texture format ", static_cast<int>(format));
	}
}


void OpenGLTexture2D::apply()
{
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(magFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(horizontalWrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(verticalWrap));
}