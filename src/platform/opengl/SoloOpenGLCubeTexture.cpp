#include "SoloOpenGLCubeTexture.h"
#include "SoloOpenGLHelper.h"

using namespace solo;


GLenum convertToGLCubeTextureFace(CubeTextureFace face)
{
    switch (face)
    {
    case CubeTextureFace::Front:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    case CubeTextureFace::Back:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
    case CubeTextureFace::Right:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    case CubeTextureFace::Left:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    case CubeTextureFace::Top:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    case CubeTextureFace::Bottom:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    default:
        SL_THROW_FMT(EngineException, "Unexpected cube texture face ", static_cast<int>(face));
    }
}


OpenGLCubeTexture::OpenGLCubeTexture()
{
    glGenTextures(1, &handle);
    if (!handle)
        SL_THROW_FMT(EngineException, "Failed to obtain texture handle");
}


OpenGLCubeTexture::~OpenGLCubeTexture()
{
    glDeleteTextures(1, &handle);
}


void OpenGLCubeTexture::setData(CubeTextureFace face, ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height)
{
    auto glFace = convertToGLCubeTextureFace(face);
    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(glFace, 0, OpenGLHelper::convertToGLColorFormat(format), width, height, 0, OpenGLHelper::convertToGLColorFormat(format), GL_UNSIGNED_BYTE, data.size() ? data.data() : 0);
    unbind();
}


void OpenGLCubeTexture::bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
}


void OpenGLCubeTexture::unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


void OpenGLCubeTexture::apply()
{
    bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, OpenGLHelper::convertToGLFilter(minFiltering));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, OpenGLHelper::convertToGLFilter(magFiltering));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, OpenGLHelper::convertToGLWrapMode(horizontalWrapping));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, OpenGLHelper::convertToGLWrapMode(verticalWrapping));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, OpenGLHelper::convertToGLWrapMode(depthWrapping));
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}


void OpenGLCubeTexture::generateMipmaps()
{
    bind();
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    unbind();
}


GLuint OpenGLCubeTexture::getHandle() const
{
    return handle;
}
