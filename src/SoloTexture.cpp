#include "SoloTexture.h"
#include "SoloEngine.h"
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


void Texture::setWrapping(TextureWrapping wrap)
{
	horizontalWrapping = wrap;
	verticalWrapping = wrap;
}