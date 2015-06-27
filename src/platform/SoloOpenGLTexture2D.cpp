#include "SoloOpenGLTexture2D.h"

using namespace solo;


OpenGLTexture2D::OpenGLTexture2D()
{
	glGenTextures(1, &handle);
	if (!handle)
		THROW_FMT(EngineException, "Failed to obtain texture handle.");
}


OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &handle);
}


void OpenGLTexture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, handle);
}


void OpenGLTexture2D::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


GLenum OpenGLTexture2D::toGLColorFormat(ColorFormat format)
{
	switch (format)
	{
		case ColorFormat::RGB:
			return GL_RGB;
		case ColorFormat::RGBA:
			return GL_RGBA;
		default:
			THROW_FMT(EngineException, "Unexpected texture format ", static_cast<int>(format), ".");
	}
}


GLenum OpenGLTexture2D::toGLWrapMode(WrapMode mode)
{
	switch (mode)
	{
		case WrapMode::Clamp:
			return GL_CLAMP_TO_EDGE;
		case WrapMode::Repeat:
			return GL_REPEAT;
		default:
			THROW_FMT(EngineException, "Unexpected wrap mode ", static_cast<int>(mode), ".");
	}
}


GLenum OpenGLTexture2D::toGLFilter(Filter filter)
{
	switch (filter)
	{
		case Filter::Linear:
			return GL_LINEAR;
		case Filter::Nearest:
			return GL_NEAREST;
		case Filter::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case Filter::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		case Filter::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case Filter::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		default:
			THROW_FMT(EngineException, "Unexpected texture filter ", static_cast<int>(filter));
	}
}


void OpenGLTexture2D::apply()
{
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilter(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilter(magFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLWrapMode(horizontalWrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLWrapMode(verticalWrap));
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}


void OpenGLTexture2D::setData(ColorFormat format, const std::vector<byte> &data, unsigned width, unsigned height)
{
	bind();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, toGLColorFormat(format), width, height, 0, toGLColorFormat(format), GL_UNSIGNED_BYTE, data.size() ? data.data() : 0);
	size = { static_cast<float>(width), static_cast<float>(height) };
	unbind();
}


void OpenGLTexture2D::generateMipmaps()
{
	bind();
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glGenerateMipmap(GL_TEXTURE_2D);
	unbind();
}


void* OpenGLTexture2D::getNativeHandle()
{
	return &handle;
}