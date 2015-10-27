#include "SoloCubeTexture.h"

using namespace solo;


TextureWrapMode CubeTexture::getDepthWrapMode() const
{
	return depthWrap;
}


void CubeTexture::setDepthWrapMode(TextureWrapMode depthWrap)
{
	this->depthWrap = depthWrap;
}