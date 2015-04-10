#include "SoloTexture.h"
#include "platform/SoloOpenGLTexture2D.h"

using namespace solo;


shared<Texture2D> TextureFactory::create2D(Texture::ColorFormat format, std::vector<byte> data, unsigned width, unsigned height, bool generateMipmaps)
{
	return NEW2(OpenGLTexture2D, format, data, width, height, generateMipmaps);
}


void Texture::setWrapMode(WrapMode verticalWrap, WrapMode horizontalWrap)
{
	this->verticalWrap = verticalWrap;
	this->horizontalWrap = horizontalWrap;
}


void Texture::setFilterMode(Filter minFilter, Filter magFilter)
{
	this->minFilter = minFilter;
	this->magFilter = magFilter;
}
