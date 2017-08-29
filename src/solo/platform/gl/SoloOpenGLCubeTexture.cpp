/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLCubeTexture.h"

#ifdef SL_OPENGL_RENDERER

using namespace solo;

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

void gl::CubeTexture::setData(CubeTextureFace face, TextureFormat format, const void *data, uint32_t width, uint32_t height)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    
    auto glFace = toCubeTextureFace(face);
    auto internalFormat = toInternalTextureFormat(format);
    auto fmt = toTextureFormat(format);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(glFace, 0, internalFormat, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

#endif
