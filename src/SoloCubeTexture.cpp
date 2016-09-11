/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloCubeTexture.h"

using namespace solo;


auto CubeTexture::create() -> sptr<CubeTexture>
{
    return std::shared_ptr<CubeTexture>(new CubeTexture());
}


CubeTexture::CubeTexture()
{
    rebuildFlags();
}


void CubeTexture::bind()
{
    renderer->setCubeTexture(handle, flags, anisotropy);
}


void CubeTexture::generateMipmaps()
{
    renderer->generateCubeTextureMipmaps(handle);
}


void CubeTexture::setData(CubeTextureFace face, TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height)
{
    renderer->updateCubeTexture(handle, face, format, width, height, data);
}


void CubeTexture::setWrapping(TextureWrapping wrap)
{
    verticalWrapping = wrap;
    horizontalWrapping = wrap;
    depthWrapping = wrap;
    rebuildFlags();
}


void CubeTexture::rebuildFlags()
{
    Texture::rebuildFlags();

    switch (depthWrapping)
    {
        case TextureWrapping::Clamp: flags |= TextureFlags::DepthWrapClamp; break;
        case TextureWrapping::Repeat: flags |= TextureFlags::DepthWrapRepeat; break;
        default: break;
    }
}
