/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLTexture2d.h"

#ifdef SL_OPENGL_RENDERER

using namespace solo;

gl::Texture2d::Texture2d(uint32_t width, uint32_t height, TextureFormat format):
    solo::Texture2d(width, height, format)
{
    std::vector<uint32_t> data;
    setData(data.data());
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

#endif
