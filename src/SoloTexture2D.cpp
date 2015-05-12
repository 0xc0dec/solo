#pragma once

#include "SoloTexture2D.h"

using namespace solo;


void Texture2D::setWrapMode(WrapMode verticalWrap, WrapMode horizontalWrap)
{
	this->verticalWrap = verticalWrap;
	this->horizontalWrap = horizontalWrap;
}


void Texture2D::setFilterMode(Filter minFilter, Filter magFilter)
{
	this->minFilter = minFilter;
	this->magFilter = magFilter;
}


void Texture2D::setAnisotropyLevel(float level)
{
	anisotropy = level;
}