#include "SoloOpenGLCubeTexture.h"
#include "SoloOpenGLHelper.h"

using namespace solo;


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
    auto glFace = OpenGLHelper::convertCubeTextureFace(face);
    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(glFace, 0, OpenGLHelper::convertColorFormat(format), width, height, 0, OpenGLHelper::convertColorFormat(format), GL_UNSIGNED_BYTE, data.size() ? data.data() : 0);
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
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, OpenGLHelper::convertTextureFiltering(minFiltering));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, OpenGLHelper::convertTextureFiltering(magFiltering));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, OpenGLHelper::convertTextureWrapping(horizontalWrapping));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, OpenGLHelper::convertTextureWrapping(verticalWrapping));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, OpenGLHelper::convertTextureWrapping(depthWrapping));
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
