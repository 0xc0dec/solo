#include "SoloCubeTexture.h"

using namespace solo;


void CubeTexture::setWrapMode(TextureWrapMode wrap)
{
	verticalWrap = wrap;
	horizontalWrap = wrap;
	depthWrap = wrap;
}