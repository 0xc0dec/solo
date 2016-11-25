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

#include "SoloTexture.h"

using namespace solo;


Texture::Texture()
{
    rebuildFlags(); // yes, virtual call
}


void Texture::setWrapping(TextureWrapping wrap)
{
    horizontalWrapping = wrap;
    verticalWrapping = wrap;
    rebuildFlags();
}


void Texture::rebuildFlags()
{
    flags = 0;

    switch (horizontalWrapping)
    {
    case TextureWrapping::Clamp:
        flags |= TextureFlags::HorizontalWrapClamp;
        break;
    case TextureWrapping::Repeat:
        flags |= TextureFlags::HorizontalWrapRepeat;
        break;
    default:
        break;
    }

    switch (verticalWrapping)
    {
    case TextureWrapping::Clamp:
        flags |= TextureFlags::VerticalWrapClamp;
        break;
    case TextureWrapping::Repeat:
        flags |= TextureFlags::VerticalWrapRepeat;
        break;
    default:
        break;
    }

    switch (minFiltering)
    {
    case TextureFiltering::Nearest:
        flags |= TextureFlags::MinFilterNearest;
        break;
    case TextureFiltering::Linear:
        flags |= TextureFlags::MinFilterLinear;
        break;
    case TextureFiltering::NearestMipmapNearest:
        flags |= TextureFlags::MinFilterNearestMipmapNearest;
        break;
    case TextureFiltering::LinearMipmapNearest:
        flags |= TextureFlags::MinFilterLinearMipmapNearest;
        break;
    case TextureFiltering::NearestMipmapLinear:
        flags |= TextureFlags::MinFilterNearestMipmapLinear;
        break;
    case TextureFiltering::LinearMipmapLinear:
        flags |= TextureFlags::MinFilterLinearMipmapLinear;
        break;
    default:
        break;
    }

    switch (magFiltering)
    {
    case TextureFiltering::Nearest:
        flags |= TextureFlags::MagFilterNearest;
        break;
    case TextureFiltering::Linear:
        flags |= TextureFlags::MagFilterLinear;
        break;
    case TextureFiltering::NearestMipmapNearest:
        flags |= TextureFlags::MagFilterNearestMipmapNearest;
        break;
    case TextureFiltering::LinearMipmapNearest:
        flags |= TextureFlags::MagFilterLinearMipmapNearest;
        break;
    case TextureFiltering::NearestMipmapLinear:
        flags |= TextureFlags::MagFilterNearestMipmapLinear;
        break;
    case TextureFiltering::LinearMipmapLinear:
        flags |= TextureFlags::MagFilterLinearMipmapLinear;
        break;
    default:
        break;
    }
}

