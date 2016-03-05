#include "SoloCubeTexture.h"

using namespace solo;


CubeTexture::CubeTexture(Renderer* renderer):
    Texture(renderer)
{
}


void CubeTexture::apply()
{
    renderer->setCubeTexture(handle, flags, anisotropy);
}


void CubeTexture::setData(CubeTextureFace face, ColorFormat format, const std::vector<uint8_t>& data, int width, int height)
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
        case TextureWrapping::Clamp: flags |= DepthWrapClamp; break;
        case TextureWrapping::Repeat: flags |= DepthWrapRepeat; break;
        default: break;
    }
}
