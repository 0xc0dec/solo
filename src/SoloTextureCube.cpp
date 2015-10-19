#include "SoloTextureCube.h"

using namespace solo;


TextureWrapMode TextureCube::getDepthWrapMode() const
{
	return depthWrap;
}


void TextureCube::setDepthWrapMode(TextureWrapMode depthWrap)
{
	this->depthWrap = depthWrap;
}