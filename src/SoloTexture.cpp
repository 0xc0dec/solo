#include "SoloTexture.h"
#include "SoloStubTexture2D.h"
#include "SoloStubTextureCube.h"
#include "SoloEngineCreationArgs.h"
#include "platform/SoloOpenGLTexture2D.h"
#include "platform/SoloOpenGLCubeTexture.h"

using namespace solo;


shared<Texture2D> TextureFactory::create2D(EngineMode mode)
{
	if (mode == EngineMode::OpenGL)
		return SL_NEW2(OpenGLTexture2D);
	return SL_NEW2(StubTexture2D);
}


shared<TextureCube> TextureFactory::createCube(EngineMode mode)
{
	if (mode == EngineMode::OpenGL)
		return SL_NEW2(OpenGLTextureCube);
	return SL_NEW2(StubTextureCube);
}


TextureWrapMode Texture::getVerticalWrapMode() const
{
	return verticalWrap;
}


void Texture::setVerticalWrapMode(TextureWrapMode verticalWrap)
{
	this->verticalWrap = verticalWrap;
}


TextureWrapMode Texture::getHorizontalWrapMode() const
{
	return horizontalWrap;
}


void Texture::setHorizontalWrapMode(TextureWrapMode horizontalWrap)
{
	this->horizontalWrap = horizontalWrap;
}


TextureFilter Texture::getMinFilter() const
{
	return minFilter;
}


void Texture::setMinFilter(TextureFilter minFilter)
{
	this->minFilter = minFilter;
}


TextureFilter Texture::getMagFilter() const
{
	return magFilter;
}


void Texture::setMagFilter(TextureFilter magFilter)
{
	this->magFilter = magFilter;
}


float Texture::getAnisotropyLevel() const
{
	return anisotropy;
}


void Texture::setAnisotropyLevel(float level)
{
	anisotropy = level;
}
