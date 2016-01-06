#include "SoloCubeTexture.h"

using namespace solo;


void CubeTexture::setWrapping(TextureWrapping wrap)
{
    verticalWrapping = wrap;
    horizontalWrapping = wrap;
    depthWrapping = wrap;
}