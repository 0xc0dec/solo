#include "SoloOpenGLRenderer.h"
#include "SoloOpenGLHelper.h"

using namespace solo;


OpenGLRenderer::OpenGLRenderer()
{
}


TextureHandle OpenGLRenderer::createTexture()
{
    GLuint rawHandle = 0;
    glGenTextures(1, &rawHandle);
    // TODO validate this new handle

    auto handle = textureHandles.reserveHandle();
    textureHandles.getData(handle) = rawHandle;

    return handle;
}


void OpenGLRenderer::destroyTexture(TextureHandle handle)
{
    // TODO validate handle
    auto rawHandle = handle == EmptyTextureHandle ? 0 : textureHandles.getData(handle);
    glDeleteTextures(1, &rawHandle);
    textureHandles.releaseHandle(handle);
}


void OpenGLRenderer::update2DTexture(TextureHandle handle, ColorFormat format, int width, int height, const std::vector<uint8_t>& data)
{
    bindTexture(GL_TEXTURE_2D, handle);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        OpenGLHelper::convertColorFormat(format),
        width,
        height,
        0,
        OpenGLHelper::convertColorFormat(format),
        GL_UNSIGNED_BYTE,
        data.size() ? data.data() : 0);

    bindTexture(GL_TEXTURE_2D, EmptyTextureHandle);
}


void OpenGLRenderer::updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height, const std::vector<uint8_t>& data)
{
    bindTexture(GL_TEXTURE_CUBE_MAP, handle);

    auto glFace = OpenGLHelper::convertCubeTextureFace(face);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        glFace,
        0,
        OpenGLHelper::convertColorFormat(format),
        width,
        height,
        0,
        OpenGLHelper::convertColorFormat(format),
        GL_UNSIGNED_BYTE,
        data.size() ? data.data() : 0);

    bindTexture(GL_TEXTURE_CUBE_MAP, EmptyTextureHandle);
}


void OpenGLRenderer::bindTexture(GLenum target, TextureHandle handle)
{
    // TODO validate handle
    auto rawHandle = handle == EmptyTextureHandle ? 0 : textureHandles.getData(handle);
    glBindTexture(target, rawHandle);
}


void OpenGLRenderer::setTexture(GLenum target, TextureHandle handle, TextureFlags flags)
{
    bindTexture(target, handle);

    if (!flags)
        return;

    GLenum minFilter = 0;
    if (flags & MinFilterLinear)
        minFilter = GL_LINEAR;
    else if (flags & MinFilterLinearMipmapLinear)
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & MinFilterLinearMipmapNearest)
        minFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & MinFilterNearest)
        minFilter = GL_NEAREST;
    else if (flags & MinFilterNearestMipmapLinear)
        minFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & MinFilterNearestMipmapNearest)
        minFilter = GL_NEAREST_MIPMAP_NEAREST;
    if (minFilter)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

    GLenum magFilter = 0;
    if (flags & MagFilterLinear)
        magFilter = GL_LINEAR;
    else if (flags & MagFilterLinearMipmapLinear)
        magFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & MagFilterLinearMipmapNearest)
        magFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & MagFilterNearest)
        magFilter = GL_NEAREST;
    else if (flags & MagFilterNearestMipmapLinear)
        magFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & MagFilterNearestMipmapNearest)
        magFilter = GL_NEAREST_MIPMAP_NEAREST;
    if (magFilter)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    GLenum wrapS = 0;
    if (flags & HorizontalWrapClamp)
        wrapS = GL_CLAMP_TO_BORDER;
    else if (flags & HorizontalWrapRepeat)
        wrapS = GL_REPEAT;
    if (wrapS)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);

    GLenum wrapT = 0;
    if (flags & VerticalWrapClamp)
        wrapT = GL_CLAMP_TO_BORDER;
    else if (flags & VerticalWrapRepeat)
        wrapT = GL_REPEAT;
    if (wrapT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}


OpenGLRenderer::~OpenGLRenderer()
{
    // TODO free resources
}


void OpenGLRenderer::set2DTexture(TextureHandle handle)
{
    bindTexture(GL_TEXTURE_2D, handle);
}


void OpenGLRenderer::set2DTexture(TextureHandle handle, TextureFlags flags)
{
    setTexture(GL_TEXTURE_2D, handle, flags);
}


void OpenGLRenderer::set2DTexture(TextureHandle handle, TextureFlags flags, float anisotropyLevel)
{
    set2DTexture(handle, flags);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}


void OpenGLRenderer::setCubeTexture(TextureHandle handle)
{
    bindTexture(GL_TEXTURE_CUBE_MAP, handle);
}


void OpenGLRenderer::setCubeTexture(TextureHandle handle, TextureFlags flags)
{
    setTexture(GL_TEXTURE_CUBE_MAP, handle, flags);

    GLenum wrapR = 0;
    if (flags & DepthWrapClamp)
        wrapR = GL_CLAMP_TO_BORDER;
    else if (flags & DepthWrapRepeat)
        wrapR = GL_REPEAT;
    if (wrapR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
}


void OpenGLRenderer::setCubeTexture(TextureHandle handle, TextureFlags flags, float anisotropyLevel)
{
    setCubeTexture(handle, flags);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}
