/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLTexture.h"

#ifdef SL_OPENGL_RENDERER

using namespace solo;

gl::Texture::Texture()
{
    glGenTextures(1, &handle);
    SL_PANIC_IF(!handle, "Failed to create texture handle");
}

gl::Texture::~Texture()
{
    glDeleteTextures(1, &handle);
}

#endif
