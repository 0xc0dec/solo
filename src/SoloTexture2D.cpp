#pragma once

#include "SoloTexture2D.h"

using namespace solo;


void Texture2D::setData(ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height)
{
	applyData(format, data, width, height);
	size = { static_cast<float>(width), static_cast<float>(height) };
}


Vector2 Texture2D::getSize() const
{
	return size;
}


WrapMode Texture2D::getVerticalWrapMode() const
{
	return verticalWrap;
}


WrapMode Texture2D::getHorizontalWrapMode() const
{
	return horizontalWrap;
}


void Texture2D::setWrapMode(WrapMode verticalWrap, WrapMode horizontalWrap)
{
	this->verticalWrap = verticalWrap;
	this->horizontalWrap = horizontalWrap;
}


Filter Texture2D::getMinFilter() const
{
	return minFilter;
}


Filter Texture2D::getMagFilter() const
{
	return magFilter;
}


void Texture2D::setFilterMode(Filter minFilter, Filter magFilter)
{
	this->minFilter = minFilter;
	this->magFilter = magFilter;
}


float Texture2D::getAnisotropyLevel() const
{
	return anisotropy;
}


void Texture2D::setAnisotropyLevel(float level)
{
	anisotropy = level;
}