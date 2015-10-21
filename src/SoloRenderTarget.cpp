#include "SoloStubRenderTarget.h"
#include "SoloEngineCreationArgs.h"
#include "platform/SoloOpenGLRenderTarget.h"

using namespace solo;


shared<RenderTarget> RenderTargetFactory::create(EngineMode mode)
{
	if (mode == EngineMode::OpenGL)
		return SL_NEW2(OpenGLRenderTarget);
	return SL_NEW2(StubRenderTarget);
}


void RenderTarget::setTextures(const std::vector<shared<Texture2D>>& textures)
{
	this->textures = textures;
	update();
}


std::vector<shared<Texture2D>> RenderTarget::getTextures() const
{
	return textures;
}