/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLCubeTexture.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloTextureData.h"
#include <algorithm>

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

gl::CubeTexture::CubeTexture(uint32_t width, uint32_t height, uint32_t depth, TextureFormat format):
    solo::CubeTexture(width, height, depth, format)
{
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

    for (int i = 0; i < depth; ++i)
    {
        const auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + i);
        const auto glFace = toTextureCubeMapFace(face);
        const auto internalFormat = toInternalTextureFormat(data->getFormat()); // TODO pass 'face' instead of 'i'
        const auto fmt = toTextureFormat(data->getFormat());
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(glFace, 0, internalFormat, data->getWidth(i, 0), data->getHeight(i, 0), 0, fmt, GL_UNSIGNED_BYTE, data->getData(i));
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

#endif
