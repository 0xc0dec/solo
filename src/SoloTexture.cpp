#include "SoloTexture.h"
#include "SoloStubTexture2D.h"
#include "SoloEngineCreationArgs.h"
#include "platform/SoloOpenGLTexture2D.h"

using namespace solo;


shared<Texture2D> TextureFactory::create2D(EngineMode mode)
{
	if (mode == EngineMode::OpenGL)
		return NEW2(OpenGLTexture2D);
	return NEW2(StubTexture2D);
}