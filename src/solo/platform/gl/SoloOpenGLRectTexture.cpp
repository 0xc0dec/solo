/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLRectTexture.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGL.h"

using namespace solo;


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


void gl::RectTexture::setData(TextureFormat format, const void *data, uint32_t width, uint32_t height)
{
    glBindTexture(GL_TEXTURE_2D, handle);

    auto internalFormat = toInternalTextureFormat(format);
    auto fmt = toTextureFormat(format);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    size = {static_cast<float>(width), static_cast<float>(height)};
}

#endif
