#include "SoloOpenGLTextureCube.h"
#include "SoloOpenGLHelper.h"

using namespace solo;


GLenum convertToGLCubeTextureFace(TextureCubeFace face)
{
	switch (face)
	{
		case TextureCubeFace::Front:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case TextureCubeFace::Back:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		case TextureCubeFace::Right:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case TextureCubeFace::Left:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case TextureCubeFace::Top:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case TextureCubeFace::Bottom:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		default:
			SL_THROW_FMT(EngineException, "Unexpected cube texture face ", static_cast<int>(face));
	}
}


OpenGLTextureCube::OpenGLTextureCube()
{
	glGenTextures(1, &handle);
	if (!handle)
		SL_THROW_FMT(EngineException, "Failed to obtain texture handle");
}


OpenGLTextureCube::~OpenGLTextureCube()
{
	glDeleteTextures(1, &handle);
}


void OpenGLTextureCube::setData(TextureCubeFace face, ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height)
{
	auto glFace = convertToGLCubeTextureFace(face);
	bind();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(glFace, 0, OpenGLHelper::convertToGLColorFormat(format), width, height, 0, OpenGLHelper::convertToGLColorFormat(format), GL_UNSIGNED_BYTE, data.size() ? data.data() : 0);
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


void OpenGLTextureCube::apply()
{
	bind();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, OpenGLHelper::convertToGLFilter(minFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, OpenGLHelper::convertToGLFilter(magFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, OpenGLHelper::convertToGLWrapMode(horizontalWrap));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, OpenGLHelper::convertToGLWrapMode(verticalWrap));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, OpenGLHelper::convertToGLWrapMode(depthWrap));
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
