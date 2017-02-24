/*
    Copyright (c) Aleksey Fedotov
    MIT license
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

