/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLRectTexture.h"

#ifdef SL_OPENGL_RENDERER

using namespace solo;

gl::RectTexture::RectTexture(uint32_t width, uint32_t height, TextureFormat format):
    solo::RectTexture(width, height, format)
{
    std::vector<uint32_t> data;
    setData(data.data());
}

void gl::RectTexture::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);

    applyMinFilter(GL_TEXTURE_2D, flags);
    applyMagFilter(GL_TEXTURE_2D, flags);
    applyRectWrap(GL_TEXTURE_2D, flags);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}

void gl::RectTexture::generateMipmaps()
{
    glBindTexture(GL_TEXTURE_2D, handle);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void gl::RectTexture::setData(const void *data)
{
    glBindTexture(GL_TEXTURE_2D, handle);

    const auto internalFormat = toInternalTextureFormat(format);
    const auto fmt = toTextureFormat(format);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, fmt, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

#endif
