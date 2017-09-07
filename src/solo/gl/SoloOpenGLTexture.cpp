/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLTexture.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloTextureData.h"

using namespace solo;

static auto toTextureCubeMapFace(CubeTextureFace face) -> GLenum
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

gl::Texture::Texture()
{
    glGenTextures(1, &handle);
    SL_PANIC_IF(!handle, "Failed to create texture handle");
}

gl::Texture::~Texture()
{
    glDeleteTextures(1, &handle);
}

gl::Texture2d::Texture2d(Texture2dData *data):
    solo::Texture2d(data)
{
    setData(data->getData());
}

void gl::Texture2d::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);

    applyMinFilter(GL_TEXTURE_2D, flags);
    applyMagFilter(GL_TEXTURE_2D, flags);
    applyRectWrap(GL_TEXTURE_2D, flags);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}

void gl::Texture2d::generateMipmaps()
{
    glBindTexture(GL_TEXTURE_2D, handle);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void gl::Texture2d::setData(const void *data)
{
    glBindTexture(GL_TEXTURE_2D, handle);

    const auto internalFormat = toInternalTextureFormat(format);
    const auto fmt = toTextureFormat(format);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dimensions.x, dimensions.y, 0, fmt, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

gl::CubeTexture::CubeTexture(CubeTextureData *data):
    solo::CubeTexture(data)
{
    setData(data);
}

void gl::CubeTexture::bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    applyMinFilter(GL_TEXTURE_CUBE_MAP, flags);
    applyMagFilter(GL_TEXTURE_CUBE_MAP, flags);
    applyRectWrap(GL_TEXTURE_CUBE_MAP, flags);
    applyDepthWrap(flags);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}

void gl::CubeTexture::generateMipmaps()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void gl::CubeTexture::setData(CubeTextureData *data)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    for (int i = 0; i < 6; ++i)
    {
        const auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + i);
        const auto glFace = toTextureCubeMapFace(face);
        const auto internalFormat = toInternalTextureFormat(data->getFormat()); // TODO pass 'face' instead of 'i'
        const auto fmt = toTextureFormat(data->getFormat());
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // TODO Add support for mip levels
        glTexImage2D(glFace, 0, internalFormat, data->getDimension(), data->getDimension(), 0, fmt, GL_UNSIGNED_BYTE, data->getData(i));
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

#endif
