/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLTexture.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloTextureData.h"

using namespace solo;

static auto toFormat(TextureFormat format) -> GLenum
{
    switch (format)
    {
        case TextureFormat::Red:
            return GL_RED;
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        default:
            SL_PANIC("Unknown image format");
            return GL_RED;
    }
}

static auto toInternalFormat(TextureFormat format) -> GLenum
{
    switch (format)
    {
        case TextureFormat::Red:
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        default:
            SL_PANIC("Unknown image format");
            return GL_RGB;
    }
}

static void applyRectWrap(GLenum target, u32 flags)
{
    GLenum wrapS = 0;
    if (flags & TextureFlags::HorizontalWrapClamp)
        wrapS = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::HorizontalWrapRepeat)
        wrapS = GL_REPEAT;
    if (wrapS)
        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);

    GLenum wrapT = 0;
    if (flags & TextureFlags::VerticalWrapClamp)
        wrapT = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::VerticalWrapRepeat)
        wrapT = GL_REPEAT;
    if (wrapT)
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
}

static void applyDepthWrap(u32 flags)
{
    GLenum wrapR = 0;
    if (flags & TextureFlags::DepthWrapClamp)
        wrapR = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::DepthWrapRepeat)
        wrapR = GL_REPEAT;
    if (wrapR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
}

static void applyMinFilter(GLenum target, u32 flags)
{
    GLenum minFilter = 0;
    if (flags & TextureFlags::MinFilterLinear)
        minFilter = GL_LINEAR;
    else if (flags & TextureFlags::MinFilterLinearMipmapLinear)
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MinFilterLinearMipmapNearest)
        minFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & TextureFlags::MinFilterNearest)
        minFilter = GL_NEAREST;
    else if (flags & TextureFlags::MinFilterNearestMipmapLinear)
        minFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MinFilterNearestMipmapNearest)
        minFilter = GL_NEAREST_MIPMAP_NEAREST;

    if (minFilter)
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
}

static void applyMagFilter(GLenum target, u32 flags)
{
    GLenum magFilter = 0;
    if (flags & TextureFlags::MagFilterLinear)
        magFilter = GL_LINEAR;
    else if (flags & TextureFlags::MagFilterLinearMipmapLinear)
        magFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MagFilterLinearMipmapNearest)
        magFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & TextureFlags::MagFilterNearest)
        magFilter = GL_NEAREST;
    else if (flags & TextureFlags::MagFilterNearestMipmapLinear)
        magFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MagFilterNearestMipmapNearest)
        magFilter = GL_NEAREST_MIPMAP_NEAREST;

    if (magFilter)
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
}

static auto toCubeMapFace(CubeTextureFace face) -> GLenum
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
            SL_PANIC("Unknown cube texture face");
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    }
}

gl::OpenGLTexture::OpenGLTexture()
{
    glGenTextures(1, &handle);
    SL_PANIC_IF(!handle, "Failed to create texture handle");
}

gl::OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &handle);
}

gl::OpenGLTexture2d::OpenGLTexture2d(Texture2dData *data):
    Texture2d(data)
{
    setData(data->getData());
}

void gl::OpenGLTexture2d::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);

    applyMinFilter(GL_TEXTURE_2D, flags);
    applyMagFilter(GL_TEXTURE_2D, flags);
    applyRectWrap(GL_TEXTURE_2D, flags);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}

void gl::OpenGLTexture2d::generateMipmaps()
{
    glBindTexture(GL_TEXTURE_2D, handle);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void gl::OpenGLTexture2d::setData(const void *data)
{
    glBindTexture(GL_TEXTURE_2D, handle);

    const auto internalFormat = toInternalFormat(this->format);
    const auto format = toFormat(this->format);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dimensions.x, dimensions.y, 0, format, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

gl::OpenGLCubeTexture::OpenGLCubeTexture(CubeTextureData *data):
    CubeTexture(data)
{
    setData(data);
}

void gl::OpenGLCubeTexture::bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    applyMinFilter(GL_TEXTURE_CUBE_MAP, flags);
    applyMagFilter(GL_TEXTURE_CUBE_MAP, flags);
    applyRectWrap(GL_TEXTURE_CUBE_MAP, flags);
    applyDepthWrap(flags);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}

void gl::OpenGLCubeTexture::generateMipmaps()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void gl::OpenGLCubeTexture::setData(CubeTextureData *data)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    for (int i = 0; i < 6; ++i)
    {
        const auto face = static_cast<CubeTextureFace>(static_cast<u32>(CubeTextureFace::Front) + i);
        const auto glFace = toCubeMapFace(face);
        const auto internalFormat = toInternalFormat(data->getFormat()); // TODO pass 'face' instead of 'i'
        const auto fmt = toFormat(data->getFormat());
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // TODO Add support for mip levels
        glTexImage2D(glFace, 0, internalFormat, data->getDimension(), data->getDimension(), 0, fmt, GL_UNSIGNED_BYTE, data->getData(i));
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

#endif
