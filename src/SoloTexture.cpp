#include "SoloTexture.h"
#include "SoloEngineCreationArgs.h"
#include "platform/stub/SoloStubTexture2D.h"
#include "platform/stub/SoloStubCubeTexture.h"
#include "platform/opengl/SoloOpenGLTexture2D.h"
#include "platform/opengl/SoloOpenGLCubeTexture.h"

using namespace solo;


shared<Texture2D> TextureFactory::create2D(EngineMode mode)
{
	if (mode == EngineMode::OpenGL)
		return SL_NEW2(OpenGLTexture2D);
	return SL_NEW2(StubTexture2D);
}


shared<CubeTexture> TextureFactory::createCube(EngineMode mode)
{
	if (mode == EngineMode::OpenGL)
		return SL_NEW2(OpenGLCubeTexture);
	return SL_NEW2(StubCubeTexture);
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
