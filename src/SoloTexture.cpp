#include "SoloTexture.h"

using namespace solo;


Texture::Texture(Renderer* renderer) :
    renderer(renderer)
{
    handle = renderer->createTexture();
}


Texture::~Texture()
{
    if (handle != EmptyTextureHandle)
        renderer->destroyTexture(handle);
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
        case TextureWrapping::Clamp: flags |= HorizontalWrapClamp; break;
        case TextureWrapping::Repeat: flags |= HorizontalWrapRepeat; break;
        default: break;
    }

    switch (verticalWrapping)
    {
        case TextureWrapping::Clamp: flags |= VerticalWrapClamp; break;
        case TextureWrapping::Repeat: flags |= VerticalWrapRepeat; break;
        default: break;
    }

    switch (minFiltering)
    {
        case TextureFiltering::Nearest: flags |= MinFilterNearest; break;
        case TextureFiltering::Linear: flags |= MinFilterLinear; break;
        case TextureFiltering::NearestMipmapNearest: flags |= MinFilterNearestMipmapNearest; break;
        case TextureFiltering::LinearMipmapNearest: flags |= MinFilterLinearMipmapNearest; break;
        case TextureFiltering::NearestMipmapLinear: flags |= MinFilterNearestMipmapLinear; break;
        case TextureFiltering::LinearMipmapLinear: flags |= MinFilterLinearMipmapLinear; break;
        default: break;
    }

    switch (magFiltering)
    {
        case TextureFiltering::Nearest: flags |= MagFilterNearest; break;
        case TextureFiltering::Linear: flags |= MagFilterLinear; break;
        case TextureFiltering::NearestMipmapNearest: flags |= MagFilterNearestMipmapNearest; break;
        case TextureFiltering::LinearMipmapNearest: flags |= MagFilterLinearMipmapNearest; break;
        case TextureFiltering::NearestMipmapLinear: flags |= MagFilterNearestMipmapLinear; break;
        case TextureFiltering::LinearMipmapLinear: flags |= MagFilterLinearMipmapLinear; break;
        default: break;
    }
}

