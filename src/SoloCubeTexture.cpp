#include "SoloCubeTexture.h"
#include "SoloBitFlags.h"

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
