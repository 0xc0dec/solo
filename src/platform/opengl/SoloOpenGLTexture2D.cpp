#include "SoloOpenGLTexture2D.h"
#include "SoloOpenGLHelper.h"

using namespace solo;


OpenGLTexture2D::OpenGLTexture2D()
{
    glGenTextures(1, &handle);
    if (!handle)
        SL_THROW_FMT(EngineException, "Failed to obtain texture handle");
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


void OpenGLTexture2D::apply()
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OpenGLHelper::convertTextureFiltering(minFiltering));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, OpenGLHelper::convertTextureFiltering(magFiltering));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OpenGLHelper::convertTextureWrapping(horizontalWrapping));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OpenGLHelper::convertTextureWrapping(verticalWrapping));
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}


void OpenGLTexture2D::applyData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height)
{
    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, OpenGLHelper::convertColorFormat(format), width, height, 0, OpenGLHelper::convertColorFormat(format), GL_UNSIGNED_BYTE, data.size() ? data.data() : 0);
    unbind();
}


void OpenGLTexture2D::generateMipmaps()
{
    bind();
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}


GLuint OpenGLTexture2D::getHandle() const
{
    return handle;
}
