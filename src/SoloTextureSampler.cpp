#include "SoloTextureSampler.h"
#include "platform/SoloOpenGLTextureSampler2D.h"

using namespace solo;


shared<TextureSampler> TextureSamplerFactory::create(shared<Texture> texture)
{
	return NEW2(OpenGLTextureSampler2D, texture);
}


TextureSampler::TextureSampler(shared<Texture> texture):
	texture(texture)
{
}


void TextureSampler::setWrapMode(WrapMode verticalWrap, WrapMode horizontalWrap)
{
	this->verticalWrap = verticalWrap;
	this->horizontalWrap = horizontalWrap;
}


void TextureSampler::setFilterMode(Filter minFilter, Filter magFilter)
{
	this->minFilter = minFilter;
	this->magFilter = magFilter;
}
