#include "SoloOpenGLTextureCube.h"

using namespace solo;


OpenGLTextureCube::OpenGLTextureCube()
{
	glGenTextures(1, &handle);
	if (!handle)
		THROW_FMT(EngineException, "Failed to obtain texture handle");
}


OpenGLTextureCube::~OpenGLTextureCube()
{
	glDeleteTextures(1, &handle);
}


void OpenGLTextureCube::setImageData(TextureCubeFace face, ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height)
{
	GLenum glFace;
	switch (face)
	{
	case TextureCubeFace::Front:
		glFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		break;
	case TextureCubeFace::Back:
		glFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		break;
	case TextureCubeFace::Left:
		glFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		break;
	case TextureCubeFace::Right:
		glFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		break;
	case TextureCubeFace::Top:
		glFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		break;
	case TextureCubeFace::Bottom:
		glFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		break;
	default:
		THROW_FMT(EngineException, "Unexpected cube texture face ", static_cast<int>(face));
	}

	bind();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(glFace, 0, toGLColorFormat(format), width, height, 0, toGLColorFormat(format), GL_UNSIGNED_BYTE, data.size() ? data.data() : 0);
	unbind();
}


void OpenGLTextureCube::bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
}


void OpenGLTextureCube::unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


GLenum OpenGLTextureCube::toGLColorFormat(ColorFormat format)
{
	switch (format)
	{
		case ColorFormat::RGB:
			return GL_RGB;
		case ColorFormat::RGBA:
			return GL_RGBA;
		default:
			THROW_FMT(EngineException, "Unexpected texture format ", static_cast<int>(format));
	}
}


GLenum OpenGLTextureCube::toGLWrapMode(TextureWrapMode mode)
{
	switch (mode)
	{
		case TextureWrapMode::Clamp:
			return GL_CLAMP_TO_EDGE;
		case TextureWrapMode::Repeat:
			return GL_REPEAT;
		default:
			THROW_FMT(EngineException, "Unexpected wrap mode ", static_cast<int>(mode));
	}
}


GLenum OpenGLTextureCube::toGLFilter(TextureFilter filter)
{
	switch (filter)
	{
		case TextureFilter::Linear:
			return GL_LINEAR;
		case TextureFilter::Nearest:
			return GL_NEAREST;
		case TextureFilter::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFilter::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		case TextureFilter::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureFilter::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		default:
			THROW_FMT(EngineException, "Unexpected texture filter ", static_cast<int>(filter));
	}
}


void OpenGLTextureCube::apply()
{
	bind();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, toGLFilter(minFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, toGLFilter(magFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, toGLWrapMode(horizontalWrap));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, toGLWrapMode(verticalWrap));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, toGLWrapMode(depthWrap));
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}


void OpenGLTextureCube::generateMipmaps()
{
	bind();
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	unbind();
}


GLuint OpenGLTextureCube::getHandle() const
{
	return handle;
}
